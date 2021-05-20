// Fill out your copyright notice in the Description page of Project Settings.


#include "Packet.h"

Packet::Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header)
{
	this->Packet::Packet(_buffer, _packetSize, _header, true);
}

Packet::Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header, bool _isDAllocBuf)
{
	this->header = _header;
	this->body = &_buffer[PACKET_HEAD_SIZE];
	this->buffer = _buffer;
	this->packetSize = _packetSize;
	this->isDAllocBuf = _isDAllocBuf;
}

Packet::~Packet()
{
	if (isDAllocBuf && IsValid())
	{
		delete[] buffer;
	}
}

PROTOCOL Packet::GetHeader() const
{
	return header;
}

char* Packet::GetBody() const
{
	return body;
}

char* Packet::GetBuffer() const
{
	return buffer;
}

ULONG Packet::GetPacketSize() const
{
	return packetSize;
}

bool Packet::IsValid() const
{
	return packetSize > 0 && buffer != nullptr;
}