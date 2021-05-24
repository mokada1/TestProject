// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPDefine.h"
#include "Protocol.h"

/**
 * 
 */
class Packet
{
public:
	Packet(const Packet& rhs);
	Packet& operator=(const Packet& rhs);
	Packet(Packet&& rhs) noexcept;
	Packet& operator=(Packet&& rhs) noexcept;
	Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header);
	Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header, bool _isDAllocBuf);
	~Packet();

	PROTOCOL GetHeader() const;
	char* GetBody() const;
	char* GetBuffer() const;
	ULONG GetPacketSize() const;

	bool IsValid() const;
	void Clear();

private:
	void Init(char* const _buffer, ULONG _packetSize, PROTOCOL _header, bool _isDAllocBuf);
	void Alloc(const Packet& rhs);

	PROTOCOL header;
	char* body;
	char* buffer;
	ULONG packetSize;
	bool isDAllocBuf;
};
