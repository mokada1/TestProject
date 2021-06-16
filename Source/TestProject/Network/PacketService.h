#pragma once

#include "../Util/TSingleton.h"
#include "../Util/Delegate.h"
#include "Struct/BcastMove.h"

class Packet;
class UObjUser;

class PacketService : public TSingleton<PacketService>
{
public:
	void Process(const Packet& packet);

	Delegate<const FString&> recvCallError;
	Delegate<const TArray<UObjUser*>&> recvCallResLogin;
	Delegate<const UObjUser* const> recvCallBcastEnterGameRoom;
	Delegate<const FString&> recvCallBcastExitGameRoom;
	Delegate<const FBcastMove&> recvCallBcastMove;
};