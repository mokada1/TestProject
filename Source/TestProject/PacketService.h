#pragma once

#include "TSingleton.h"
#include "Packet.h"
#include "Delegate.h"

class ATPClient;

class PacketService : public TSingleton<PacketService>
{
public:
	void Process(const Packet& packet);

	Delegate<> delGameRoomObj;

private:
	ATPClient* client;	
};