#include "PacketGenerator.h"
#include "../Session/Session.h"
#include "../../Util/TPLogger.h"

vector<Packet*> PacketGenerator::Parse(Session* const owner, char* const buffer, const size_t recvBytes)
{
	vector<Packet*> packetList;
	size_t recvBytesRemain = recvBytes;
	char* bufferRemain = buffer;
	bool isDAllocBufferRemain = false;

	// ���ŵ� ����Ʈ��ŭ ��Ŷ ����
	while (recvBytesRemain > 0)
	{
		char* finishedBuffer = nullptr;
		size_t finishedPacketSize = 0;

		auto ownerBuff = owner->GetBuffer();
		if (ownerBuff)
		{
			owner->AddToBuff(bufferRemain, recvBytesRemain);
			ownerBuff = owner->GetBuffer();
			const auto ownerPacketSize = owner->GetPacketSize();

			const auto header = GetHeaderByBuff(ownerBuff);
			// �߸��� Header�� ��� ����
			if (!IsValidHeader(header))
			{
				TPLogger::GetInstance().PrintLog(INVALID_HEADER);
				owner->ClearBuff();
				break;
			}

			size_t packetSize = 0;
			const auto endOfPacket = GetEndOfPacket(ownerBuff, ownerPacketSize, packetSize);
			if (!IsValidEndOfPacket(endOfPacket))
			{
				// �߸��� EndOfPacket�� ��� ����
				if (ownerPacketSize == MAX_BUFF_SIZE)
				{
					TPLogger::GetInstance().PrintLog(INVALID_END_OF_PACKET);
					owner->ClearBuff();
					break;
				}
			}
			else
			{
				// ��Ŷ �ϼ�
				finishedBuffer = new char[packetSize];
				memcpy(finishedBuffer, ownerBuff, packetSize);
				finishedPacketSize = packetSize;

				// ��Ŷ �ϼ� �� ���� ����Ʈ ���
				recvBytesRemain = ownerPacketSize - packetSize;
				if (recvBytesRemain > 0)
				{
					auto temp = new char[recvBytesRemain];
					memcpy(temp, &ownerBuff[packetSize], recvBytesRemain);
					bufferRemain = temp;
					isDAllocBufferRemain = true;
				}
				owner->ClearBuff();
			}
		}
		else
		{
			if (recvBytesRemain < PACKET_HEAD_SIZE)
			{
				owner->AddToBuff(bufferRemain, recvBytesRemain);
			}
			else
			{
				const auto header = GetHeaderByBuff(bufferRemain);
				// �߸��� Header�� ��� ����
				if (!IsValidHeader(header))
				{
					TPLogger::GetInstance().PrintLog(INVALID_HEADER);
					break;
				}

				size_t packetSize = 0;
				const auto endOfPacket = GetEndOfPacket(bufferRemain, recvBytesRemain, packetSize);
				if (!IsValidEndOfPacket(endOfPacket))
				{
					// �߸��� EndOfPacket�� ��� ����
					if (recvBytesRemain == MAX_BUFF_SIZE)
					{
						TPLogger::GetInstance().PrintLog(INVALID_END_OF_PACKET);
						break;
					}
					owner->AddToBuff(bufferRemain, recvBytesRemain);
				}
				else
				{
					// ��Ŷ �ϼ�
					finishedBuffer = new char[packetSize];
					memcpy(finishedBuffer, bufferRemain, packetSize);
					finishedPacketSize = packetSize;

					// ��Ŷ �ϼ� �� ���� ����Ʈ ���
					recvBytesRemain -= packetSize;
					if (recvBytesRemain > 0)
					{
						if (isDAllocBufferRemain)
						{
							delete[] bufferRemain;
							isDAllocBufferRemain = false;
						}
						bufferRemain = &bufferRemain[packetSize];
					}
				}
			}
		}

		// ��Ŷ�� �ϼ����� �ʾ����� ����
		if (!finishedBuffer)
		{
			break;
		}

		// �ϼ��� ��Ŷ ����Ʈ�� �߰�
		PROTOCOL header = GetHeaderByBuff(finishedBuffer);
		auto packetInfo = PacketInfo(finishedBuffer, finishedPacketSize, header);
		auto packetSubInfo = PacketSubInfo(owner, PacketCastType::UNICAST, vector<Session*>(), true);
		auto newPacket = new Packet(packetInfo, packetSubInfo);
		packetList.emplace_back(newPacket);
	}

	// ���� �Ҵ�� �ӽ� ���۰� ���� ������ ����
	if (isDAllocBufferRemain)
	{
		delete[] bufferRemain;
		isDAllocBufferRemain = false;
	}

	return packetList;
}

Packet PacketGenerator::CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner)
{
	return CreatePacket(header, _fbb, owner, PacketCastType::UNICAST, vector<Session*>());
}

Packet PacketGenerator::CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner, PacketCastType packetCastType)
{
	return CreatePacket(header, _fbb, owner, packetCastType, vector<Session*>());
}

Packet PacketGenerator::CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner, PacketCastType packetCastType, vector<Session*> packetCastGroup)
{
	auto bp = _fbb.GetBufferPointer();
	auto bSize = static_cast<size_t>(_fbb.GetSize());

	const size_t INTER_BUFF_DATA_SIZE = PACKET_HEAD_SIZE + bSize;
	const size_t BUFF_DATA_SIZE = PACKET_HEAD_SIZE + bSize + PACKET_END_SIZE;

	auto buffer = new char[BUFF_DATA_SIZE];
	memset(buffer, 0, BUFF_DATA_SIZE);

	SetHeaderOfBuff(buffer, header);
	memcpy(&buffer[PACKET_HEAD_SIZE], bp, bSize);
	SetEndOfBuff(buffer, INTER_BUFF_DATA_SIZE);

	auto packetInfo = PacketInfo(buffer, BUFF_DATA_SIZE, header);
	auto packetSubInfo = PacketSubInfo(owner, packetCastType, packetCastGroup);
	return Packet(packetInfo, packetSubInfo);
}

PROTOCOL PacketGenerator::GetHeaderByBuff(char* const buffer)
{
	unsigned char byte1 = buffer[0];
	unsigned char byte2 = buffer[1];
	uint16_t protoInt16 = byte1 | byte2 << 8;
	return static_cast<PROTOCOL>(protoInt16);
}

PROTOCOL PacketGenerator::GetEndOfPacket(char* const buffer, const size_t buffSize, size_t& packetSize)
{
	PROTOCOL protocol = PROTOCOL::TP_ERROR;
	for (int i = 0; i < buffSize - 1; ++i)
	{
		unsigned char byte1 = buffer[i];
		unsigned char byte2 = buffer[i + 1];
		uint16_t protoInt16 = byte1 | byte2 << 8;
		protocol = static_cast<PROTOCOL>(protoInt16);
		if (IsValidEndOfPacket(protocol))
		{
			packetSize = static_cast<size_t>(i) + 2;
			break;
		}
	}
	return protocol;
}

void PacketGenerator::SetHeaderOfBuff(char* const buffer, PROTOCOL header)
{
	uint16_t protoInt16 = static_cast<uint16_t>(header);
	unsigned char byte1 = protoInt16 & 0xFF;
	unsigned char byte2 = protoInt16 >> 8;
	buffer[0] = byte1;
	buffer[1] = byte2;
}

void PacketGenerator::SetEndOfBuff(char* const buffer, const size_t buffSize)
{
	uint16_t protoInt16 = static_cast<uint16_t>(PROTOCOL::END_OF_PACKET);
	unsigned char byte1 = protoInt16 & 0xFF;
	unsigned char byte2 = protoInt16 >> 8;
	buffer[buffSize] = byte1;
	buffer[buffSize + 1] = byte2;
}

bool PacketGenerator::IsValidHeader(const PROTOCOL protocol)
{
	switch (protocol)
	{
	case PROTOCOL::TP_ERROR:
	case PROTOCOL::RES_LOGIN:
	case PROTOCOL::RES_ROUND_TRIP_TIME:
	case PROTOCOL::BCAST_ENTER_GAME_ROOM:
	case PROTOCOL::BCAST_EXIT_GAME_ROOM:
	case PROTOCOL::BCAST_MOVE:
	case PROTOCOL::BCAST_LOCATION_SYNC:
	case PROTOCOL::BCAST_ACTION:
	case PROTOCOL::BCAST_HIT:
	case PROTOCOL::BCAST_ROTATION_SYNC:
		return true;
	}
	return false;
}

bool PacketGenerator::IsValidEndOfPacket(const PROTOCOL protocol)
{
	return protocol == PROTOCOL::END_OF_PACKET;
}
