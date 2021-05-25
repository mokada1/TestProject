#include "ATPClient.h"
#include "TPError.h"
#include "PacketGenerator.h"
#include "TPUtil.h"
#include "PacketService.h"

#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>

void ATPClient::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    Close();
}

bool ATPClient::CreateClientAndConnect(const FString _serverIp, const FString _serverPort)
{
    if (isConnected)
    {
        return false;
    }

    if (!Initialize(_serverIp, _serverPort))
    {
        return false;
    }

    if (!Connect())
    {
        return false;
    }

    isConnected = true;

    SetRecvCallback();

    return true;
}

bool ATPClient::Initialize(const FString _serverIp, const FString _serverPort)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        TPError::GetInstance().PrintError(L"WSAStartup() Error!");
        return false;
    }

    hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (hSocket == INVALID_SOCKET)
    {
        TPError::GetInstance().PrintError(L"Socket() Error");
        return false;
    }
    
    char hServerIp[SERVER_IP_SIZE];
	TPUtil::GetInstance().WCharToMultiByte(hServerIp, SERVER_IP_SIZE, *_serverIp);
    serverIp = hServerIp;
    serverPort = FCString::Atoi(*_serverPort);

    memset(&recvAddr, 0, sizeof(recvAddr));
    recvAddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIp, &(recvAddr.sin_addr));
    recvAddr.sin_port = htons(serverPort);

    return true;
}

bool ATPClient::Connect()
{
    if (connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
    {
        TPError::GetInstance().PrintError(L"Connect() Error!");
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
    ClearRecvCallback();
    return true;
}

void ATPClient::SetRecvCallback()
{       
    PacketService::GetInstance().recvCallGameRoomObj += std::bind(&ATPClient::CallGameRoomObj, this);
}

void ATPClient::ClearRecvCallback()
{
    PacketService::GetInstance().recvCallGameRoomObj.clear();
}

bool ATPClient::ReqLogin(const FString _userId, const FString _password)
{
	if (isLogined)
	{
		return false;
	}

    char hUserId[SIZE_USER_USER_ID], hPassword[SIZE_USER_PASSWORD];
    TPUtil::GetInstance().WCharToMultiByte(hUserId, SIZE_USER_USER_ID, *_userId);
    TPUtil::GetInstance().WCharToMultiByte(hPassword, SIZE_USER_PASSWORD, *_password);

    auto packet = PacketGenerator::GetInstance().CreateReqLogin(hUserId, hPassword);
    return rsThread->SendPacket(packet);
}

void ATPClient::CallGameRoomObj()
{
    if (!isLogined)
    {
        isLogined = true;
    }
    K2_RecvCallGameRoomObj();
}
