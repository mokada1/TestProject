#pragma once

#include "Protocol.h"
#include "../../Util/TPDefine.h"

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
	size_t GetPacketSize() const;

	bool IsValid() const;
	void Clear();

private:
	void Alloc(const Packet& rhs);

	PROTOCOL header;
	char* body;
	char* buffer;
	size_t packetSize;
	bool isDAllocBuf;
};

class PacketInfo
{
public:
	PacketInfo(char* const _buffer, size_t _packetSize, PROTOCOL _header)
	{
		buffer = _buffer;
		packetSize = _packetSize;
		header = _header;
	}

	char* GetBuffer() const { return buffer; }
	size_t GetPacketSize() const { return packetSize; }
	PROTOCOL GetHeader() const { return header; }

private:
	char* buffer;
	size_t packetSize;
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