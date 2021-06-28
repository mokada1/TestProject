#include "PacketProcessor.h"
#include "../Core/TPClient.h"
#include "PacketGenerator.h"
#include "PacketService.h"

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
	auto packet = PacketGenerator::GetInstance().Parse(client->GetSession(), buffer, recvBytes);
	if (!packet)
	{
		return;
	}
	if (!packet->IsValid())
	{
		delete packet;
		return;
	}
	packetRecvQueue.Enqueue(packet);
}

bool PacketProcessor::SendPacket(Packet* const packet)
{
	if (!client)
	{
		return false;
	}
	packetSendQueue.Enqueue(packet);
	return true;
}

bool PacketProcessor::ProcRecvPacket()
{
	if (packetRecvQueue.IsEmpty())
	{
		return false;
	}
	Packet* packet = nullptr;
	if (packetRecvQueue.Dequeue(packet))
	{
		PacketService::GetInstance().Process(*packet);
		delete packet;
		return true;
	}
	return false;
}

bool PacketProcessor::ProcSendPacket()
{
	if (!client || packetSendQueue.IsEmpty())
	{
		return false;
	}
	bool result = false;
	Packet* packet = nullptr;
	if (packetSendQueue.Dequeue(packet))
	{
		result = client->SendPacket(*packet);
		delete packet;
	}
	return result;
}

void PacketProcessor::Close()
{
	while (!packetSendQueue.IsEmpty())
	{
		Packet* packet = nullptr;
		if (packetSendQueue.Dequeue(packet))
		{
			delete packet;
		}
	}
	while (!packetRecvQueue.IsEmpty())
	{
		Packet* packet = nullptr;
		if (packetRecvQueue.Dequeue(packet))
		{
			delete packet;
		}
	}
}
