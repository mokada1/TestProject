#pragma once

#include <WinSock2.h>

class Session
{
public:
	Session();
	Session(SOCKET servSock, SOCKADDR_IN servAddr);
	~Session();

	void ClearBuff();
	void ClearBuff(bool isDeleteBuff);
	void AddToBuff(char* const _buffer, const ULONG _packetSize);

	void SetUserId(const wchar_t* const _userId);
	void SetBuffer(char* const _buffer);
	void SetPacketSize(const ULONG _packetSize);

	SOCKET GetServSock() const;	
	wchar_t* GetUserId() const;
	char* GetBuffer() const;
	ULONG GetPacketSize() const;

	bool IsValid() const;
private:
	SOCKET hServSock;
	const char* ip;
	wchar_t* userId;
	char* buffer;
	ULONG packetSize;
};