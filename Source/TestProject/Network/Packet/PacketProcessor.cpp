#include "PacketProcessor.h"
#include "../Core/TPClient.h"
#include "PacketGeneratorClient.h"
#include "PacketService.h"
#include "../../Util/TPLogger.h"

void PacketProcessor::SetClient(ATPClient* const _client)
{
	client = _client;
}

void PacketProcessor::Parse(char* const buffer, const size_t recvBytes)
{
	if (!client)
	{
		return;
	}

	auto packetList = PacketGeneratorClient::GetInstance().Parse(client->GetSession(), buffer, recvBytes);
	if (packetList.empty())
	{
		return;
	}

	for (auto packet : packetList)
	{
		auto header = packet->GetHeader();
		auto strHeader = TPUtil::GetInstance().EnumToString(header);

		TPLogger::GetInstance().PrintLog(RECV_PACKET_1, strHeader);

		packetQueue.Enqueue(packet);
	}
}

bool PacketProcessor::Process()
{
	if (!client || packetQueue.IsEmpty())
	{
		return false;
	}
	Packet* packet = nullptr;
	if (packetQueue.Dequeue(packet))
	{
		PacketService::GetInstance().Process(*packet);
		delete packet;
		return true;
	}
	return false;
}

bool PacketProcessor::SendPacket(const Packet& packet)
{
	if (!client)
	{
		return false;
	}
	return client->SendPacket(packet);
}
