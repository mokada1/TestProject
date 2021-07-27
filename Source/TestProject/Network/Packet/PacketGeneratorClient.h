#pragma once

#include "../../Util/Unconstructible.h"
#include "../Packet/PacketGenerator.h"
#include "../Struct/BcastMove.h"
#include "../Struct/BcastAction.h"
#include "../Struct/BcastRotationSync.h"
#include <string>

using namespace std;

class PacketGeneratorClient : public PacketGenerator, public Unconstructible
{
public:
	static PacketGeneratorClient& GetInstance()
	{
		static PacketGeneratorClient* _instance = new PacketGeneratorClient();
		return *_instance;
	}

	Packet CreateReqLogin(const string& userId, const string& password);
	Packet CreateReqMove(FBcastMove& bcastMove);
	Packet CreateReqLocationSync(const FVector& location);
	Packet CreateReqRoundTripTime();
	Packet CreateReqAction(FBcastAction& bcastAction);
	Packet CreateReqAbility(EAbilityType abilityType, const FVector& location, const FVector& rotation);
	Packet CreateReqRotationSync(const FVector& rotation);

private:
	PacketGeneratorClient() {}
};