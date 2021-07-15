#include "TPClient.h"
#include "../../Util/TPLogger.h"
#include "../Packet/PacketGenerator.h"
#include "../../Util/TPUtil.h"
#include "../Packet/PacketService.h"
#include "../Packet/PacketProcessor.h"
#include "../Session/Session.h"

#include "Runtime/Core/Public/Windows/AllowWindowsPlatformTypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#include "Runtime/Core/Public/Windows/HideWindowsPlatformTypes.h"

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
        TPLogger::GetInstance().PrintLog("WSAStartup() Error!");
        return false;
    }

    hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (hSocket == INVALID_SOCKET)
    {
        TPLogger::GetInstance().PrintLog("Socket() Error");
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
        TPLogger::GetInstance().PrintLog("Connect() Error!");
        return false;
    }

    session = new Session(hSocket, recvAddr);

    rsThread = NewObject<USocketRSThread>();
    rsThread->Start(hSocket);

    PacketProcessor::GetInstance().SetClient(this);

    TPLogger::GetInstance().PrintLog("Connect success");

    return true;
}

void ATPClient::CallEndPlay(const EEndPlayReason::Type endPlayReason)
{
	Close();
}

void ATPClient::Close()
{
    isConnected = false;
    PacketProcessor::GetInstance().SetClient(nullptr);

    if (session)
    {
        delete session;
        session = nullptr;
    }

	if (hSocket)
	{
        if (rsThread && rsThread->IsRunning())
        {
            rsThread->Stop();
            rsThread = nullptr;
        }
        closesocket(hSocket);
	}
    
    WSACleanup();
}

bool ATPClient::GetIsConnected() const
{
    return isConnected;
}

void ATPClient::Process()
{
    while(PacketProcessor::GetInstance().Process());
}

bool ATPClient::SendPacket(const Packet& packet)
{
    if (!rsThread || !rsThread->IsRunning())
    {
        return false;
    }
	return rsThread->SendPacket(packet);
}

Session* ATPClient::GetSession() const
{
    return session;
}
