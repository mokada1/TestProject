#include "SocketRSThread.h"
#include "TP_generated.h"
#include "TPError.h"

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

void USocketRSThread::Start(SOCKET socket)
{
	this->hSocket = socket;
	FRunnableThread::Create(this, TEXT("USocketRSThread"));
}

bool USocketRSThread::SendPacket(char* datas, int dataSize)
{
	WSAEVENT event = WSACreateEvent();
	WSAOVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = event;

	WSABUF dataBuf;
	dataBuf.len = dataSize;
	dataBuf.buf = datas;
	int sendBytes = 0;
	int flags = 0;

	if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
	{
		auto e = WSAGetLastError();
		if (e != WSA_IO_PENDING)
		{
			TPError::GetInstance().PrintError(L"WSASend() error", e);
			return false;
		}
	}

	WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE);

	WSAGetOverlappedResult(hSocket, &overlapped, (LPDWORD)&sendBytes, FALSE, (LPDWORD)&flags);

	UE_LOG(LogTemp, Log, TEXT("Number of bytes transferred: %d"), sendBytes);
	
	return true;
}

bool USocketRSThread::RecvPacket()
{
	WSAEVENT event = WSACreateEvent();
	WSAOVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = event;

	char buffer[1024];
	WSABUF dataBuf;
	dataBuf.len = 1024;
	dataBuf.buf = buffer;
	int recvBytes = 0;
	int flags = 0;

	if (WSARecv(hSocket, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL) == SOCKET_ERROR)
	{
		auto e = WSAGetLastError();
		if (e != WSA_IO_PENDING)
		{
			TPError::GetInstance().PrintError(L"WSARecv() error", e);
			return false;
		}
	}

	WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE);

	WSAGetOverlappedResult(hSocket, &overlapped, (LPDWORD)&recvBytes, FALSE, (LPDWORD)&flags);

	UE_LOG(LogTemp, Log, TEXT("Number of bytes received: %d"), recvBytes);
	
	if (recvBytes > 0)
	{
		auto datas = dataBuf.buf;
		uint16_t headerInt16 = datas[0] | static_cast<uint16_t>(datas[1]) << 8;
		PROTOCOL header = static_cast<PROTOCOL>(headerInt16);

		auto body = const_cast<char*>(&datas[HEAD_SIZE]);

		auto req = flatbuffers::GetRoot<REQ_MOVE>(body);
		auto charId = req->CharacterId();
		auto pos = req->Pos();
		
		UE_LOG(LogTemp, Log, TEXT("CharacterId:%d Pos:%f %f %f"), charId, pos->x(), pos->y(), pos->z());
	}
	return true;
}