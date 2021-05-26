#pragma once

#include "TSingleton.h"
#include "Packet.h"
#include "Delegate.h"

class PacketService : public TSingleton<PacketService>
{
public:
	void Process(const Packet& packet);

	Delegate<const FString&> recvCallError;
	Delegate<> recvCallGameRoomObj;
	Delegate<> recvCallEnterGameRoom;
	Delegate<> recvCallExitGameRoom;
};