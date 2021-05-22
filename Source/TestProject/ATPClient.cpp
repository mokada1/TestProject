#include "ATPClient.h"
#include "TPError.h"
#include "PacketGenerator.h"
#include "TPUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>

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

bool ATPClient::SendTestPacket(const FString userId, const FString password)
{
    char hUserId[SIZE_USER_USER_ID], hPassword[SIZE_USER_PASSWORD];
    TPUtil::GetInstance().WCharToMultiByte(hUserId, SIZE_USER_USER_ID, *userId);
    TPUtil::GetInstance().WCharToMultiByte(hPassword, SIZE_USER_PASSWORD, *password);

    auto packet = PacketGenerator::GetInstance().CreateReqLogin(hUserId, hPassword);
    return rsThread->SendPacket(packet);
}