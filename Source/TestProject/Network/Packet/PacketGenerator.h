#pragma once

#include "../../Util/TSingleton.h"
#include "Packet.h"
#include "../../TP_generated.h"
#include "../Struct/BcastMove.h"
#include <string>

using namespace std;

class Session;

class PacketGenerator : public TSingleton<PacketGenerator>
{
public:
	Packet* Parse(Session* const owner, char* const buffer, const size_t recvBytes);

	Packet* CreateReqLogin(const string& userId, const string& password);
	Packet* CreateReqMove(FBcastMove& moveLocation);
	Packet* CreateReqMoveSync(const string& userId, const FVector& location);
	Packet* CreateReqRoundTripTime();
	
private:	
	Packet* CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb);
	PROTOCOL GetHeaderByBuff(char* const buffer);
	PROTOCOL GetEndOfPacket(char* const buffer, const size_t packetSize);
	void SetHeaderOfBuff(char* const buffer, PROTOCOL header);
	void SetEndOfBuff(char* const buffer, const size_t buffSize);
	bool IsValidHeader(const PROTOCOL protocol);
	bool IsValidEndOfPacket(const PROTOCOL protocol);
};