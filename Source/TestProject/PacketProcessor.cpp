#include "PacketProcessor.h"
#include "TPClient.h"
#include "PacketGenerator.h"
#include "PacketService.h"

void PacketProcessor::SetClient(ATPClient* const _client)
{
	client = _client;
}

void PacketProcessor::PushToPacketList(char* const buffer, const DWORD bytesTransferred)
{
	const auto& packet = PacketGenerator::GetInstance().Parse(buffer, bytesTransferred);
	packetList.push(packet);
}

bool PacketProcessor::Process()
{
	if (packetList.empty())
	{
		return false;
	}
	PacketService::GetInstance().Process(packetList.front());
	packetList.pop();
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
