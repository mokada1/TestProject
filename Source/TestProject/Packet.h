// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Uncopyable.h"
#include "TPDefine.h"
#include "Protocol.h"

/**
 * 
 */
class TESTPROJECT_API Packet : public Uncopyable
{
public:
	Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header);
	Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header, bool _isDAllocBuf);
	~Packet();

	PROTOCOL GetHeader() const;
	char* GetBody() const;
	char* GetBuffer() const;
	ULONG GetPacketSize() const;

	bool IsValid() const;

private:
	PROTOCOL header;
	char* body;
	char* buffer;
	ULONG packetSize;
	bool isDAllocBuf;
};
