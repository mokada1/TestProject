#include "PacketProcessor.h"
#include "PacketGenerator.h"
#include "PacketService.h"

void PacketProcessor::Process(char* const buffer, const DWORD bytesTransferred)
{
	auto packet = PacketGenerator::GetInstance().Parse(buffer, bytesTransferred);
	PacketService::GetInstance().Process(*packet);
	delete packet;
}