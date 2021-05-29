#pragma once

#include "SocketRSThread.h"
#include "GameFramework/Actor.h"
#include <cstdint>
#include "TPClient.generated.h"

class Session;

UCLASS(BlueprintType, Blueprintable)
class TESTPROJECT_API ATPClient : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Network)
	bool CreateClientAndConnect(const FString& _serverIp, const FString& _serverPort);

	UFUNCTION(BlueprintCallable, Category = Network)
	void ProcessPackets();

	bool SendPacket(const Packet& packet);

	Session* GetSession() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Network)
	bool isConnected;

	UFUNCTION(BlueprintCallable, Category = TPClient)
	void CallEndPlay(const EEndPlayReason::Type endPlayReason);

private:
	bool Initialize(const FString& _serverIp, const FString& _serverPort);
	bool Connect();
	bool Close();	

	char* serverIp;
	uint8_t serverPort;
	
	SOCKET hSocket;
	SOCKADDR_IN recvAddr;
	Session* session;

	USocketRSThread* rsThread;
};