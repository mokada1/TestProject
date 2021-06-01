#pragma once

#include "TSingleton.h"
#include "Delegate.h"

class Packet;
class UObjUser;

class PacketService : public TSingleton<PacketService>
{
public:
	void Process(const Packet& packet);

	Delegate<const FString&> recvCallError;
	Delegate<const TArray<UObjUser*>&> recvCallGameRoomObj;
	Delegate<const UObjUser* const> recvCallEnterGameRoom;
	Delegate<const FString&> recvCallExitGameRoom;
	Delegate<const FString&, const TArray<FVector>&> recvCallMoveLocation;
};