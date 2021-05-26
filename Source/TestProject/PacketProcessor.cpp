#include "PacketProcessor.h"
#include "TPClient.h"
#include "PacketGenerator.h"
#include "PacketService.h"

void PacketProcessor::SetClient(ATPClient* const _client)
{
	client = _client;
}

void PacketProcessor::Process(char* const buffer, const DWORD bytesTransferred)
{
	auto packet = PacketGenerator::GetInstance().Parse(buffer, bytesTransferred);
	PacketService::GetInstance().Process(packet);
}

bool PacketProcessor::SendPacket(const Packet& packet)
{
	if (!client)
	{
		return false;
	}
	return client->SendPacket(packet);
}
