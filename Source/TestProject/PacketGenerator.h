#pragma once

#include "TSingleton.h"
#include "Packet.h"
#include "TP_generated.h"
#include <string>

using namespace std;

class Session;

class PacketGenerator : public TSingleton<PacketGenerator>
{
public:
	Packet Parse(Session* const owner, char* const buffer, ULONG bytesTransferred);

	Packet CreateReqLogin(const string& userId, const string& password);
	Packet CreateReqMove(const string& userId, const TArray<FVector>& locationList);
	
private:	
	Packet CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header);
	PROTOCOL GetHeaderByBuff(char* const buffer);
	PROTOCOL GetEndOfPacket(char* const buffer, const ULONG packetSize);
	void SetHeaderOfBuff(char* const buffer, PROTOCOL header);
	void SetEndOfBuff(char* const buffer, const int buffSize);
	bool IsValidHeader(const PROTOCOL protocol);
	bool IsValidEndOfPacket(const PROTOCOL protocol);
};