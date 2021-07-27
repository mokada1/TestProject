#pragma once

#include "../../Util/Unconstructible.h"
#include "Packet.h"

class ATPClient;

class PacketProcessor : public Unconstructible
{
public:
	static PacketProcessor& GetInstance()
	{
		static PacketProcessor* _instance = new PacketProcessor();
		return *_instance;
	}

	void SetClient(ATPClient* const _client);
	void Parse(char* const buffer, const size_t recvBytes);
	bool Process();
	bool SendPacket(const Packet& packet);

private:
	PacketProcessor() {}

	UPROPERTY()
	ATPClient* client;

	UPROPERTY()
	TQueue<Packet*> packetQueue;
};