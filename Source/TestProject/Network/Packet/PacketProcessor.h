#pragma once

#include "../../Util/TSingleton.h"
#include "Packet.h"

class ATPClient;

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void SetClient(ATPClient* const _client);
	void Parse(char* const buffer, const size_t recvBytes);
	bool Process();
	bool SendPacket(const Packet& packet);

private:
	UPROPERTY()
	ATPClient* client;

	UPROPERTY()
	TQueue<Packet*> packetQueue;
};