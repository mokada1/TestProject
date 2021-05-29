#pragma once

#include "Protocol.h"
#include "TPDefine.h"

class PacketInfo;
class PacketSubInfo;

class Packet
{
public:
	Packet();
	Packet(const Packet& rhs);
	Packet& operator=(const Packet& rhs);
	Packet(Packet&& rhs) noexcept;
	Packet& operator=(Packet&& rhs) noexcept;
	Packet(const PacketInfo& packetInfo, const PacketSubInfo& packetSubInfo);
	~Packet();

	PROTOCOL GetHeader() const;
	char* GetBody() const;
	char* GetBuffer() const;
	ULONG GetPacketSize() const;

	bool IsValid() const;
	void Clear();

private:
	void Alloc(const Packet& rhs);

	PROTOCOL header;
	char* body;
	char* buffer;
	ULONG packetSize;
	bool isDAllocBuf;
};

class PacketInfo
{
public:
	PacketInfo(char* const _buffer, ULONG _packetSize, PROTOCOL _header)
	{
		buffer = _buffer;
		packetSize = _packetSize;
		header = _header;
	}

	char* GetBuffer() const { return buffer; }
	ULONG GetPacketSize() const { return packetSize; }
	PROTOCOL GetHeader() const { return header; }

private:
	char* buffer;
	ULONG packetSize;
	PROTOCOL header;
};

class PacketSubInfo
{
public:
	PacketSubInfo(bool _isDAllocBuf)
	{
		isDAllocBuf = _isDAllocBuf;
	}

	bool GetIsDAllocBuf() const { return isDAllocBuf; }

private:
	bool isDAllocBuf;
};