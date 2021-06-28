#include "TPClient.h"
#include "../../Util/TPError.h"
#include "../Packet/PacketGenerator.h"
#include "../../Util/TPUtil.h"
#include "../Packet/PacketService.h"
#include "../Packet/PacketProcessor.h"
#include "../Session/Session.h"

#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>

bool ATPClient::CreateClientAndConnect(const FString& _serverIp, const FString& _serverPort)
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

    return true;
}

bool ATPClient::Initialize(const FString& _serverIp, const FString& _serverPort)
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

    session = new Session(hSocket, recvAddr);

    recvThread = NewObject<USocketRSThread>();
    recvThread->Start(hSocket, true);
    sendThread = NewObject<USocketRSThread>();
    sendThread->Start(hSocket, false);

    PacketProcessor::GetInstance().SetClient(this);

    UE_LOG(LogTemp, Log, TEXT("Client Connect Success"));

    return true;
}

void ATPClient::CallEndPlay(const EEndPlayReason::Type endPlayReason)
{
	Close();
    if (session)
    {
        delete session;
        session = nullptr;
    }
    PacketProcessor::GetInstance().SetClient(nullptr);
    isConnected = false;
}

bool ATPClient::Close()
{
    PacketProcessor::GetInstance().Close();
	if (hSocket)
	{
        if (recvThread && recvThread->IsRunning())
        {
            recvThread->Stop();
            recvThread = nullptr;
        }
        if (sendThread && sendThread->IsRunning())
        {
            sendThread->Stop();
            sendThread = nullptr;
        }
        closesocket(hSocket);
	}
    WSACleanup();   
    return true;
}

void ATPClient::ProcRecvPackets()
{
    while (PacketProcessor::GetInstance().ProcRecvPacket());
}

bool ATPClient::GetIsConnected() const
{
    return isConnected;
}

bool ATPClient::SendPacket(const Packet& packet)
{
    if (!sendThread || !sendThread->IsRunning())
    {
        return false;
    }
	return sendThread->SendPacket(packet);
}

Session* ATPClient::GetSession() const
{
    return session;
}
