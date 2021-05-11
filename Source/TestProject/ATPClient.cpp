#include "ATPClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#include "TP_generated.h"
#include "TPError.h"

void ATPClient::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    Close();
}

bool ATPClient::CreateClientAndConnect()
{
    if (!Initialize())
    {
        return false;
    }

    if (!Connect())
    {
        return false;
    }
    return true;
}

bool ATPClient::Initialize()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        TPError::GetInstance().PrintError(L"WSAStartup() error!");
        return false;
    }

    hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (hSocket == INVALID_SOCKET)
    {
        TPError::GetInstance().PrintError(L"socket() error");
        return false;
    }

    memset(&recvAddr, 0, sizeof(recvAddr));
    recvAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &(recvAddr.sin_addr));
    recvAddr.sin_port = htons(SERVER_PORT);

    return true;
}

bool ATPClient::Connect()
{
    if (connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
    {
        TPError::GetInstance().PrintError(L"connect() error!");
        return false;
    }

    USocketRSThread* t = NewObject<USocketRSThread>();
    rsThread = t;
    rsThread->Start(hSocket);    

    UE_LOG(LogTemp, Log, TEXT("Client Connect Success"));

    return true;
}

bool ATPClient::SendTestPacket(const int characterId, const FVector pos)
{
	// 테스트용 패킷 송신
	char datas[1024];
	uint16_t header = static_cast<uint16_t>(PROTOCOL::REQ_MOVE);
	datas[0] = header & 0xFF;
	datas[1] = header >> 8;

	flatbuffers::FlatBufferBuilder builder;
	auto vec3 = Vec3(pos.X, pos.Y, pos.Z);
	builder.Finish(CreateREQ_MOVE(builder, characterId, &vec3));
	auto bp = builder.GetBufferPointer();
	auto bSize = builder.GetSize();
	const int BODY_SIZE = bSize;
	memcpy(&datas[HEAD_SIZE], bp, BODY_SIZE);

	UE_LOG(LogTemp, Log, TEXT("HEAD_SIZE:%d BODY_SIZE:%d"), HEAD_SIZE, BODY_SIZE);

	const int DATA_SIZE = HEAD_SIZE + BODY_SIZE;

	return rsThread->SendPacket(datas, DATA_SIZE);
}

bool ATPClient::Close()
{
	if (hSocket)
	{
        if (rsThread)
        {
            rsThread->Stop();
        }
        closesocket(hSocket);
	}
    WSACleanup();
    return true;
}