// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TSingleton.h"
#include "TP_generated.h"
#include "Packet.h"
#include <string.h>

using namespace std;

class TESTPROJECT_API PacketGenerator : public TSingleton<PacketGenerator>
{
public:
	Packet* Parse(char* const buffer, DWORD bytesTransferred);
	Packet* CreateReqLogin(const char* const userId, const char* const password);

private:
	void SetHeaderOfBuff(char* const buffer, PROTOCOL header);
	Packet* CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header);
};
