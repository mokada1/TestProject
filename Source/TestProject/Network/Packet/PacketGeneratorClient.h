#pragma once

#include "../../Util/TSingleton.h"
#include "../Packet/PacketGenerator.h"
#include "../Struct/BcastMove.h"
#include "../Struct/BcastAction.h"
#include "../Struct/BcastRotationSync.h"
#include <string>

using namespace std;

class PacketGeneratorClient : public PacketGenerator, public TSingleton<PacketGeneratorClient>
{
public:
	Packet CreateReqLogin(const string& userId, const string& password);
	Packet CreateReqMove(FBcastMove& bcastMove);
	Packet CreateReqLocationSync(const FVector& location);
	Packet CreateReqRoundTripTime();
	Packet CreateReqAction(FBcastAction& bcastAction);
	Packet CreateReqAbility(EAbilityType abilityType, const FVector& location, const FVector& rotation);
	Packet CreateReqRotationSync(const FVector& rotation);
};