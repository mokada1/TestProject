#pragma once

#include "../../Util/TSingleton.h"
#include "Packet.h"
#include "Containers/Queue.h"

class ATPClient;

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void SetClient(ATPClient* const _client);
	void Parse(char* const buffer, const size_t recvBytes);
	bool SendPacket(Packet* const packet);
	bool ProcRecvPacket();
	bool ProcSendPacket();
	void Close();

private:
	UPROPERTY()
	ATPClient* client;

	UPROPERTY()
	TQueue<Packet*> packetRecvQueue;

	UPROPERTY()
	TQueue<Packet*> packetSendQueue;
};