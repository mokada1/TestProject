#include "PacketGenerator.h"
#include "Session.h"
#include "TP_generated.h"
#include "TPError.h"

#include <iostream>

using namespace std;

Packet PacketGenerator::Parse(Session* const owner, char* const buffer, ULONG bytesTransferred)
{
	if (bytesTransferred == 0)
	{
		return Packet();
	}

	char* finishedBuffer = nullptr;
	ULONG finishedPacketSize = 0;

	auto const ownerBuff = owner->GetBuffer();
	if (ownerBuff)
	{
		// �ִ� ���� ũ�⸦ �Ѵ� ��� ��Ŷ ó�� ����
		if (owner->GetPacketSize() + bytesTransferred > BUFF_SIZE)
		{
			TPError::GetInstance().PrintError(L"Error:Invalid PacketSize");
			owner->ClearBuff();
			return Packet();
		}

		owner->AddToBuff(buffer, bytesTransferred);
		const auto ownerPacketSize = owner->GetPacketSize();

		const auto header = PacketGenerator::GetInstance().GetHeaderByBuff(ownerBuff);
		// �߸��� Header�� ��� ��Ŷ ó�� ����
		if (!IsValidHeader(header))
		{
			TPError::GetInstance().PrintError(L"Error:Invalid Header");
			owner->ClearBuff();
			return Packet();
		}

		const auto endOfPacket = PacketGenerator::GetInstance().GetEndOfPacket(ownerBuff, ownerPacketSize);
		if (!IsValidEndOfPacket(endOfPacket))
		{
			// �߸��� EndOfPacket�� ��� ��Ŷ ó�� ����
			if (ownerPacketSize == BUFF_SIZE)
			{
				TPError::GetInstance().PrintError(L"Error:Invalid EndOfPacket");
				owner->ClearBuff();
				return Packet();
			}
		}
		else
		{
			// ��Ŷ �ϼ�
			finishedBuffer = ownerBuff;
			finishedPacketSize = ownerPacketSize;
			owner->ClearBuff(false);
		}
	}
	else
	{
		if (bytesTransferred < PACKET_HEAD_SIZE)
		{
			owner->AddToBuff(buffer, bytesTransferred);
		}
		else
		{
			const auto header = PacketGenerator::GetInstance().GetHeaderByBuff(buffer);
			// �߸��� Header�� ��� ��Ŷ ó�� ����
			if (!IsValidHeader(header))
			{
				TPError::GetInstance().PrintError(L"Error:Invalid Header");
				return Packet();
			}

			const auto endOfPacket = PacketGenerator::GetInstance().GetEndOfPacket(buffer, bytesTransferred);
			if (!IsValidEndOfPacket(endOfPacket))
			{
				// �߸��� EndOfPacket�� ��� ��Ŷ ó�� ����
				if (bytesTransferred == BUFF_SIZE)
				{
					TPError::GetInstance().PrintError(L"Error:Invalid EndOfPacket");
					return Packet();
				}
				owner->AddToBuff(buffer, bytesTransferred);
			}
			else
			{
				// ��Ŷ �ϼ�
				finishedBuffer = buffer;
				finishedPacketSize = bytesTransferred;
			}
		}
	}

	// ��Ŷ�� �ϼ����� �ʾ����� �� ��Ŷ ����
	if (!finishedBuffer)
	{
		return Packet();
	}

	// �ϼ��� ��Ŷ ����
	PROTOCOL header = GetHeaderByBuff(finishedBuffer);
	auto packetInfo = PacketInfo(finishedBuffer, finishedPacketSize, header);
	auto packetSubInfo = PacketSubInfo(false);
	return Packet(packetInfo, packetSubInfo);
}

Packet PacketGenerator::CreateReqLogin(const string& userId, const string& password)
{
	auto buffer = new char[BUFF_SIZE];
	memset(buffer, 0, BUFF_SIZE);

	PROTOCOL header = PROTOCOL::REQ_LOGIN;
	SetHeaderOfBuff(buffer, header);

	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(userId);
	auto offsetPassword = fbb.CreateString(password);
	fbb.Finish(CreateTB_ReqLogin(fbb, offsetUserId, offsetPassword));

	return CreatePacket(fbb, buffer, header);
}

Packet PacketGenerator::CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header)
{
	auto bp = _fbb.GetBufferPointer();
	auto bSize = _fbb.GetSize();
	memcpy(&buffer[PACKET_HEAD_SIZE], bp, bSize);

	const int INTER_BUFFER_SIZE = PACKET_HEAD_SIZE + bSize;
	SetEndOfBuff(buffer, INTER_BUFFER_SIZE);

	const int BUFFER_SIZE = PACKET_HEAD_SIZE + bSize + PACKET_END_SIZE;

	auto packetInfo = PacketInfo(buffer, BUFFER_SIZE, header);
	auto packetSubInfo = PacketSubInfo(true);
	return Packet(packetInfo, packetSubInfo);
}

PROTOCOL PacketGenerator::GetHeaderByBuff(char* const buffer)
{
	unsigned char byte1 = buffer[0];
	unsigned char byte2 = buffer[1];
	uint16_t headerInt16 = byte1 | byte2 << 8;
	return static_cast<PROTOCOL>(headerInt16);
}

PROTOCOL PacketGenerator::GetEndOfPacket(char* const buffer, const ULONG packetSize)
{
	unsigned char byte1 = buffer[packetSize - 2];
	unsigned char byte2 = buffer[packetSize - 1];
	uint16_t headerInt16 = byte1 | byte2 << 8;
	return static_cast<PROTOCOL>(headerInt16);
}

void PacketGenerator::SetHeaderOfBuff(char* const buffer, PROTOCOL header)
{
	uint16_t protoInt16 = static_cast<uint16_t>(header);
	unsigned char byte1 = protoInt16 & 0xFF;
	unsigned char byte2 = protoInt16 >> 8;
	buffer[0] = byte1;
	buffer[1] = byte2;
}

void PacketGenerator::SetEndOfBuff(char* const buffer, const int buffSize)
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
	case PROTOCOL::REQ_LOGIN:
	case PROTOCOL::REQ_MOVE:
	case PROTOCOL::GAME_ROOM_OBJ:
	case PROTOCOL::ENTER_GAME_ROOM:
	case PROTOCOL::EXIT_GAME_ROOM:
	case PROTOCOL::MOVE_LOCATION:
		return true;
	}
	return false;
}

bool PacketGenerator::IsValidEndOfPacket(const PROTOCOL protocol)
{
	return protocol == PROTOCOL::END_OF_PACKET;
}
