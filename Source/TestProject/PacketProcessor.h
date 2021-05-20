#pragma once

#include "TSingleton.h"

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void Process(char* const buffer, const DWORD bytesTransferred);
};