#include "PacketGenerator.h"
#include "../Session/Session.h"
#include "../../TP_generated.h"
#include "../../Util/TPError.h"
#include "../../Util/TPUtil.h"

#include <iostream>

using namespace std;

Packet* PacketGenerator::Parse(Session* const owner, char* const buffer, const size_t recvBytes)
{
	if (recvBytes == 0)
	{
		return nullptr;
	}

	char* finishedBuffer = nullptr;
	ULONG finishedPacketSize = 0;
	bool isDAllocBuf = false;

	auto ownerBuff = owner->GetBuffer();
	if (ownerBuff)
	{
		// 최대 버퍼 크기를 넘는 경우 패킷 처리 안함
		if (owner->GetPacketSize() + recvBytes > MAX_BUFF_SIZE)
		{
			TPError::GetInstance().PrintError(L"Error:Invalid PacketSize");
			owner->ClearBuff();
			return nullptr;
		}

		owner->AddToBuff(buffer, recvBytes);
		ownerBuff = owner->GetBuffer();
		const auto ownerPacketSize = owner->GetPacketSize();

		const auto header = PacketGenerator::GetInstance().GetHeaderByBuff(ownerBuff);
		// 잘못된 Header일 경우 패킷 처리 안함
		if (!IsValidHeader(header))
		{
			TPError::GetInstance().PrintError(L"Error:Invalid Header");
			owner->ClearBuff();
			return nullptr;
		}

		const auto endOfPacket = PacketGenerator::GetInstance().GetEndOfPacket(ownerBuff, ownerPacketSize);
		if (!IsValidEndOfPacket(endOfPacket))
		{
			// 잘못된 EndOfPacket일 경우 패킷 처리 안함
			if (ownerPacketSize == MAX_BUFF_SIZE)
			{
				TPError::GetInstance().PrintError(L"Error:Invalid EndOfPacket");
				owner->ClearBuff();
				return nullptr;
			}
		}
		else
		{
			// 패킷 완성
			finishedBuffer = ownerBuff;
			finishedPacketSize = ownerPacketSize;
			owner->ClearBuff(false);
			isDAllocBuf = true;
		}
	}
	else
	{
		if (recvBytes < PACKET_HEAD_SIZE)
		{
			owner->AddToBuff(buffer, recvBytes);
		}
		else
		{
			const auto header = PacketGenerator::GetInstance().GetHeaderByBuff(buffer);
			// 잘못된 Header일 경우 패킷 처리 안함
			if (!IsValidHeader(header))
			{
				TPError::GetInstance().PrintError(L"Error:Invalid Header");
				return nullptr;
			}

			const auto endOfPacket = PacketGenerator::GetInstance().GetEndOfPacket(buffer, recvBytes);
			if (!IsValidEndOfPacket(endOfPacket))
			{
				// 잘못된 EndOfPacket일 경우 패킷 처리 안함
				if (recvBytes == MAX_BUFF_SIZE)
				{
					TPError::GetInstance().PrintError(L"Error:Invalid EndOfPacket");
					return nullptr;
				}
				owner->AddToBuff(buffer, recvBytes);
			}
			else
			{
				// 패킷 완성
				finishedBuffer = new char[recvBytes];
				memcpy(finishedBuffer, buffer, recvBytes);
				finishedPacketSize = recvBytes;
				isDAllocBuf = true;
			}
		}
	}

	// 패킷이 완성되지 않았으면 빈 패킷 리턴
	if (!finishedBuffer)
	{
		return nullptr;
	}

	// 완성된 패킷 리턴
	PROTOCOL header = GetHeaderByBuff(finishedBuffer);
	auto packetInfo = PacketInfo(finishedBuffer, finishedPacketSize, header);
	auto packetSubInfo = PacketSubInfo(isDAllocBuf);
	return new Packet(packetInfo, packetSubInfo);
}

Packet* PacketGenerator::CreateReqLogin(const string& userId, const string& password)
{
	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(userId);
	auto offsetPassword = fbb.CreateString(password);
	fbb.Finish(CreateTB_ReqLogin(fbb, offsetUserId, offsetPassword));

	return CreatePacket(PROTOCOL::REQ_LOGIN, fbb);
}

Packet* PacketGenerator::CreateReqMove(FBcastMove& moveLocation)
{
	char hUserId[SIZE_USER_USER_ID];
	TPUtil::GetInstance().WCharToMultiByte(hUserId, SIZE_USER_USER_ID, *(moveLocation.userId));

	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(hUserId);
	auto offsetOperation = moveLocation.GetOffsetOperation();
	auto offsetInputMove = moveLocation.inputMove.Serialize(fbb);

	fbb.Finish(CreateTB_ReqMove(fbb, offsetUserId, offsetOperation, offsetInputMove));

	return CreatePacket(PROTOCOL::REQ_MOVE, fbb);
}

Packet* PacketGenerator::CreateReqMoveSync(const string& userId, const FVector& location)
{
	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(userId);
	ST_Vec3 stLocation(location.X, location.Y, location.Z);
	fbb.Finish(CreateTB_ReqLocationSync(fbb, offsetUserId, &stLocation));

	return CreatePacket(PROTOCOL::REQ_LOCATION_SYNC, fbb);
}

Packet* PacketGenerator::CreateReqRoundTripTime()
{
	flatbuffers::FlatBufferBuilder fbb;

	auto currentTimeMs = static_cast<int64_t>(TPUtil::GetInstance().TimeSinceEpochMs());
	fbb.Finish(CreateTB_ReqRoundTripTime(fbb, currentTimeMs));

	return CreatePacket(PROTOCOL::REQ_ROUND_TRIP_TIME, fbb);
}

Packet* PacketGenerator::CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb)
{
	auto bp = _fbb.GetBufferPointer();
	auto bSize = _fbb.GetSize();

	const int INTER_BUFFER_SIZE = PACKET_HEAD_SIZE + bSize;
	const int BUFF_DATA_SIZE = PACKET_HEAD_SIZE + bSize + PACKET_END_SIZE;

	auto buffer = new char[BUFF_DATA_SIZE];
	memset(buffer, 0, BUFF_DATA_SIZE);

	SetHeaderOfBuff(buffer, header);
	memcpy(&buffer[PACKET_HEAD_SIZE], bp, bSize);
	SetEndOfBuff(buffer, INTER_BUFFER_SIZE);

	auto packetInfo = PacketInfo(buffer, BUFF_DATA_SIZE, header);
	auto packetSubInfo = PacketSubInfo(true);
	return new Packet(packetInfo, packetSubInfo);
}

PROTOCOL PacketGenerator::GetHeaderByBuff(char* const buffer)
{
	unsigned char byte1 = buffer[0];
	unsigned char byte2 = buffer[1];
	uint16_t headerInt16 = byte1 | byte2 << 8;
	return static_cast<PROTOCOL>(headerInt16);
}

PROTOCOL PacketGenerator::GetEndOfPacket(char* const buffer, const size_t packetSize)
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
		return true;
	}
	return false;
}

bool PacketGenerator::IsValidEndOfPacket(const PROTOCOL protocol)
{
	return protocol == PROTOCOL::END_OF_PACKET;
}
