#pragma once

#include "../../Util/TSingleton.h"
#include "Packet.h"
#include "Containers/Queue.h"

class ATPClient;

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void SetClient(ATPClient* const _client);
	void Parse(char* const buffer, const size_t bytesTransferred);
	bool Process();
	bool SendPacket(const Packet& packet);

private:
	ATPClient* client;
	TQueue<Packet> packetQueue;
};