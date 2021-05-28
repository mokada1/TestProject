#pragma once

#include "TSingleton.h"
#include "Packet.h"
#include "Delegate.h"

class UObjUser;

class PacketService : public TSingleton<PacketService>
{
public:
	void Process(const Packet& packet);

	Delegate<const FString&> recvCallError;
	Delegate<const TArray<UObjUser*>&> recvCallGameRoomObj;
	Delegate<const UObjUser* const> recvCallEnterGameRoom;
	Delegate<const FString&> recvCallExitGameRoom;
};