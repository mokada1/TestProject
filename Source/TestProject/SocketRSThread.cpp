#include "SocketRSThread.h"
#include "TP_generated.h"
#include "TPError.h"
#include "PacketProcessor.h"

bool USocketRSThread::Init()
{
	return true;
}

uint32 USocketRSThread::Run()
{
	while (!isStopThread)
	{
		if (!RecvPacket())
		{
			break;
		}
	}
	return 0;
}

void USocketRSThread::Stop()
{	
	isStopThread = true;
}

void USocketRSThread::Exit()
{
}

void USocketRSThread::Start(SOCKET socket)
{
	this->hSocket = socket;
	FRunnableThread::Create(this, TEXT("USocketRSThread"));
}

bool USocketRSThread::SendPacket(const Packet& packet)
{
	WSAEVENT wevent = WSACreateEvent();
	WSAOVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = wevent;

	WSABUF wsaBuf;
	wsaBuf.len = packet.GetPacketSize();
	wsaBuf.buf = packet.GetBuffer();
	DWORD sendBytes = 0;
	DWORD flags = 0;

	if (WSASend(hSocket, &wsaBuf, 1, &sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
	{
		auto e = WSAGetLastError();
		if (e != WSA_IO_PENDING)
		{
			TPError::GetInstance().PrintError(L"WSASend() error", e);
			return false;
		}
	}

	WSAWaitForMultipleEvents(1, &wevent, TRUE, WSA_INFINITE, FALSE);

	WSAGetOverlappedResult(hSocket, &overlapped, &sendBytes, FALSE, &flags);

	UE_LOG(LogTemp, Log, TEXT("Number of bytes transferred: %d"), sendBytes);
	
	return true;
}

bool USocketRSThread::RecvPacket()
{
	WSAEVENT wevent = WSACreateEvent();
	WSAOVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = wevent;

	char buffer[MAX_BUFF_SIZE];
	WSABUF wsaBuf;
	wsaBuf.len = MAX_BUFF_SIZE;
	wsaBuf.buf = buffer;
	DWORD recvBytes = 0;
	DWORD flags = 0;

	if (WSARecv(hSocket, &wsaBuf, 1, &recvBytes, &flags, &overlapped, NULL) == SOCKET_ERROR)
	{
		auto e = WSAGetLastError();
		if (e != WSA_IO_PENDING)
		{
			TPError::GetInstance().PrintError(L"WSARecv() error", e);
			return false;
		}
	}

	WSAWaitForMultipleEvents(1, &wevent, TRUE, WSA_INFINITE, FALSE);

	WSAGetOverlappedResult(hSocket, &overlapped, &recvBytes, FALSE, &flags);

	UE_LOG(LogTemp, Log, TEXT("Number of bytes received: %d"), recvBytes);
	
	if (recvBytes > 0)
	{
		PacketProcessor::GetInstance().PushToPacketList(wsaBuf.buf, recvBytes);
	}
	return true;
}