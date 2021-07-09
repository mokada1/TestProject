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

		TPLogger::GetInstance().PrintLog("Recv packet:%s", strHeader);

		PacketService::GetInstance().Process(*packet);
		delete packet;
	}
}

bool PacketProcessor::SendPacket(Packet* const packet)
{
	if (!client)
	{
		return false;
	}
	auto result = client->SendPacket(*packet);
	delete packet;
	return result;
}
