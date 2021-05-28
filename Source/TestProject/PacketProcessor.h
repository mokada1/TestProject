#pragma once

#include "TSingleton.h"
#include "Packet.h"
#include <queue>

class ATPClient;

using namespace std;

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void SetClient(ATPClient* const _client);
	void PushToPacketList(char* const buffer, const DWORD bytesTransferred);
	bool Process();
	bool SendPacket(const Packet& packet);

private:
	ATPClient* client;
	queue<Packet> packetList;
};