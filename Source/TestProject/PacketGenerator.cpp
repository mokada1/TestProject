#include "PacketGenerator.h"
#include "TPUtil.h"

Packet PacketGenerator::Parse(char* const buffer, DWORD bytesTransferred)
{
	unsigned char byte1 = buffer[0];
	unsigned char byte2 = buffer[1];
	uint16_t headerInt16 = byte1 | byte2 << 8;
	PROTOCOL header = static_cast<PROTOCOL>(headerInt16);
	return Packet(buffer, bytesTransferred, header, false);
}

Packet PacketGenerator::CreateReqLogin(const char* const userId, const char* const password)
{
	auto buffer = new char[BUFSIZE];
	memset(buffer, 0, BUFSIZE);

	PROTOCOL header = PROTOCOL::REQ_LOGIN;
	SetHeaderOfBuff(buffer, header);

	flatbuffers::FlatBufferBuilder fbb;
	auto offsetUserId = fbb.CreateString(userId);
	auto offsetPassword = fbb.CreateString(password);

	fbb.Finish(CreateTB_ReqLogin(fbb, offsetUserId, offsetPassword));

	return CreatePacket(fbb, buffer, header);
}

void PacketGenerator::SetHeaderOfBuff(char* const buffer, PROTOCOL header)
{
	uint16_t protoInt16 = static_cast<uint16_t>(header);
	unsigned char byte1 = protoInt16 & 0xFF;
	unsigned char byte2 = protoInt16 >> 8;
	buffer[0] = byte1;
	buffer[1] = byte2;
}

Packet PacketGenerator::CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header)
{
	auto bp = _fbb.GetBufferPointer();
	auto bSize = _fbb.GetSize();
	memcpy(&buffer[PACKET_HEAD_SIZE], bp, bSize);
	const int BUFFER_SIZE = PACKET_HEAD_SIZE + bSize;
	return Packet(buffer, BUFFER_SIZE, header);
}