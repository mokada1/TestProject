#include "PacketGenerator.h"
#include "TPUtil.h"

Packet* PacketGenerator::Parse(char* const buffer, DWORD bytesTransferred)
{
	unsigned char byte1 = buffer[0];
	unsigned char byte2 = buffer[1];
	uint16_t headerInt16 = byte1 | byte2 << 8;
	PROTOCOL header = static_cast<PROTOCOL>(headerInt16);
	return new Packet(buffer, bytesTransferred, header, false);
}

Packet* PacketGenerator::CreateReqLogin(const wchar_t* const userId, const wchar_t* const password)
{
	char hUserId[SIZE_USER_USER_ID], hPassword[SIZE_USER_PASSWORD];;
	TPUtil::GetInstance().WCharToChar(hUserId, SIZE_USER_USER_ID, userId);
	TPUtil::GetInstance().WCharToChar(hPassword, SIZE_USER_PASSWORD, password);

	auto buffer = new char[BUFSIZE];
	memset(buffer, 0, BUFSIZE);

	PROTOCOL header = PROTOCOL::REQ_LOGIN;
	SetHeaderOfBuff(buffer, header);

	flatbuffers::FlatBufferBuilder fbb;
	auto offsetUserId = fbb.CreateString(hUserId);
	auto offsetPassword = fbb.CreateString(hPassword);

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

Packet* PacketGenerator::CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header)
{
	auto bp = _fbb.GetBufferPointer();
	auto bSize = _fbb.GetSize();
	memcpy(&buffer[PACKET_HEAD_SIZE], bp, bSize);
	const int BUFFER_SIZE = PACKET_HEAD_SIZE + bSize;
	return new Packet(buffer, BUFFER_SIZE, header);
}