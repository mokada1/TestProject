#include "SocketRSThread.h"
#include "../../TP_generated.h"
#include "../../Util/TPLogger.h"
#include "../Packet/PacketProcessor.h"
#include "../../Util/TPUtil.h"

USocketRSThread::~USocketRSThread()
{
	if (IsRunning())
	{
		delete thread;
		thread = nullptr;
	}
}

bool USocketRSThread::Init()
{
	isStopThread = false;
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

bool USocketRSThread::IsRunning()
{	
	return thread && !isStopThread;
}

void USocketRSThread::Start(SOCKET socket)
{
	this->hSocket = socket;
	thread = FRunnableThread::Create(this, TEXT("USocketRSThread"));
}

bool USocketRSThread::SendPacket(const Packet& packet)
{
	PER_IO_DATA perIoData;
	DWORD sendBytes = 0;
	DWORD flags = 0;

	memset(&(perIoData.overlapped), 0, sizeof(OVERLAPPED));
	perIoData.wsaBuf.len = static_cast<ULONG>(packet.GetPacketSize());
	perIoData.wsaBuf.buf = packet.GetBuffer();
	perIoData.operation = OP_CLIENT_TO_SERVER;

	if (WSASend(hSocket, &(perIoData.wsaBuf), 1, &sendBytes, flags, &(perIoData.overlapped), NULL) == SOCKET_ERROR)
	{
		auto e = WSAGetLastError();
		if (e != WSA_IO_PENDING)
		{
			TPLogger::GetInstance().PrintLog(WSASEND_ERROR, e);
			return false;
		}
	}

	TPLogger::GetInstance().PrintLog(TRANSFERRED_BYTES, sendBytes);

	auto header = packet.GetHeader();
	auto strHeader = TPUtil::GetInstance().EnumToString(header);

	TPLogger::GetInstance().PrintLog(SEND_PACKET_1, strHeader);
		
	return true;
}

bool USocketRSThread::RecvPacket()
{	
	PER_IO_DATA perIoData;
	DWORD recvBytes = 0;
	DWORD flags = 0;

	memset(&(perIoData.overlapped), 0, sizeof(OVERLAPPED));
	perIoData.overlapped.hEvent = WSACreateEvent();
	perIoData.wsaBuf.len = MAX_BUFF_SIZE;
	perIoData.wsaBuf.buf = perIoData.buffer;
	perIoData.operation = OP_SERVER_TO_CLIENT;

	if (WSARecv(hSocket, &(perIoData.wsaBuf), 1, &recvBytes, &flags, &(perIoData.overlapped), NULL) == SOCKET_ERROR)
	{
		auto e = WSAGetLastError();
		if (e != WSA_IO_PENDING)
		{
			TPLogger::GetInstance().PrintLog(WSARECV_ERROR, e);
			return false;
		}
	}

	WSAWaitForMultipleEvents(1, &perIoData.overlapped.hEvent, true, WSA_INFINITE, false);

	WSAGetOverlappedResult(hSocket, &perIoData.overlapped, &recvBytes, false, &flags);

	TPLogger::GetInstance().PrintLog(RECEIVED_BYTES, recvBytes);
		
	if (recvBytes > 0)
	{
		PacketProcessor::GetInstance().Parse(perIoData.wsaBuf.buf, static_cast<size_t>(recvBytes));
	}

	return true;
}