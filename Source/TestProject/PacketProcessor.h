#pragma once

#include "TSingleton.h"

class Packet;
class ATPClient;

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void SetClient(ATPClient* const _client);
	void Process(char* const buffer, const DWORD bytesTransferred);
	bool SendPacket(const Packet& packet);

private:
	ATPClient* client;
};