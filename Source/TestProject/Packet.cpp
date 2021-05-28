// Fill out your copyright notice in the Description page of Project Settings.


#include "Packet.h"

Packet::Packet(const Packet& rhs)
{
	Alloc(rhs);
}

Packet& Packet::operator=(const Packet& rhs)
{
	Alloc(rhs);
	return *this;
}

Packet::Packet(Packet&& rhs) noexcept
{
	Alloc(rhs);
	rhs.Clear();
}

Packet& Packet::operator=(Packet&& rhs) noexcept
{
	Alloc(rhs);
	rhs.Clear();
	return *this;
}

Packet::Packet()
{
	Init(nullptr, 0, PROTOCOL::TP_ERROR, true);
}

Packet::Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header)
{
	Init(_buffer, _packetSize, _header, true);
}

Packet::Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header, bool _isDAllocBuf)
{
	Init(_buffer, _packetSize, _header, _isDAllocBuf);
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

void Packet::Clear()
{
	header = PROTOCOL::TP_ERROR;
	body = nullptr;
	buffer = nullptr;
	packetSize = 0;
	isDAllocBuf = false;
}

void Packet::Init(char* const _buffer, ULONG _packetSize, PROTOCOL _header, bool _isDAllocBuf)
{
	this->header = _header;
	this->body = &_buffer[PACKET_HEAD_SIZE];
	this->buffer = _buffer;
	this->packetSize = _packetSize;
	this->isDAllocBuf = _isDAllocBuf;
}

void Packet::Alloc(const Packet& rhs)
{
	if (!rhs.IsValid())
	{
		return;
	}
	if (isDAllocBuf && IsValid())
	{
		delete[] buffer;
		buffer = nullptr;
	}
	if (rhs.isDAllocBuf)
	{
		buffer = new char[BUFSIZE];
		memcpy(buffer, rhs.buffer, BUFSIZE);
		body = &buffer[PACKET_HEAD_SIZE];
	}
	else
	{
		buffer = rhs.buffer;
		body = rhs.body;
	}
	header = rhs.header;
	packetSize = rhs.packetSize;
	isDAllocBuf = rhs.isDAllocBuf;
}
