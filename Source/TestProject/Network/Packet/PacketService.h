#pragma once

#include "../../Util/Unconstructible.h"
#include "../../Util/Delegate.h"
#include "../Struct/BcastMove.h"
#include "../Struct/BcastAction.h"
#include "../Struct/BcastRotationSync.h"
#include "../Struct/BcastLocationSync.h"

class Packet;
class UObjUser;

class PacketService : public Unconstructible
{
public:
	static PacketService& GetInstance()
	{
		static PacketService* _instance = new PacketService();
		return *_instance;
	}

	void Process(const Packet& packet);

	Delegate<const FString&> recvCallError;
	Delegate<const TArray<UObjUser*>&> recvCallResLogin;
	Delegate<const int64, const int64> recvCallResRoundTripTime;
	Delegate<const UObjUser* const> recvCallBcastEnterGameRoom;
	Delegate<const FString&> recvCallBcastExitGameRoom;
	Delegate<const FBcastMove&> recvCallBcastMove;
	Delegate<const FBcastLocationSync&> recvCallBcastLocationSync;
	Delegate<const FBcastAction&> recvCallBcastAction;
	Delegate<const TArray<UObjUser*>&> recvCallBcastHit;
	Delegate<const FBcastRotationSync&> recvCallBcastRotationSync;

private:
	PacketService() {}
};