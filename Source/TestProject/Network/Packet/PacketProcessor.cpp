#include "PacketProcessor.h"
#include "../Core/TPClient.h"
#include "PacketGenerator.h"
#include "PacketService.h"

void PacketProcessor::SetClient(ATPClient* const _client)
{
	client = _client;
}

void PacketProcessor::Parse(char* const buffer, const size_t bytesTransferred)
{
	const auto& packet = PacketGenerator::GetInstance().Parse(client->GetSession(), buffer, bytesTransferred);
	if (!packet.IsValid())
	{
		return;
	}
	packetQueue.Enqueue(packet);
}

bool PacketProcessor::Process()
{
	if (packetQueue.IsEmpty())
	{
		return false;
	}
	Packet packet;
	if (packetQueue.Dequeue(packet))
	{
		PacketService::GetInstance().Process(packet);
	}
	return true;
}

bool PacketProcessor::SendPacket(const Packet& packet)
{
	if (!client)
	{
		return false;
	}
	return client->SendPacket(packet);
}
