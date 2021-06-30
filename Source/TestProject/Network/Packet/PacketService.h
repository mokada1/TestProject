#pragma once

#include "../../Util/TSingleton.h"
#include "../../Util/Delegate.h"
#include "../Struct/BcastMove.h"
#include "../Struct/BcastInputAction.h"

class Packet;
class UObjUser;

class PacketService : public TSingleton<PacketService>
{
public:
	void Process(const Packet& packet);

	Delegate<const FString&> recvCallError;
	Delegate<const TArray<UObjUser*>&> recvCallResLogin;
	Delegate<const int64, const int64> recvCallResRoundTripTime;
	Delegate<const UObjUser* const> recvCallBcastEnterGameRoom;
	Delegate<const FString&> recvCallBcastExitGameRoom;
	Delegate<const FBcastMove&> recvCallBcastMove;
	Delegate<const FString&, const FVector&> recvCallBcastLocationSync;
	Delegate<const FBcastInputAction&> recvCallBcastInputAction;
};