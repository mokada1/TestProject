#pragma once

#include "SocketRSThread.h"
#include "GameFramework/Actor.h"
#include <cstdint>
#include "ATPClient.generated.h"

UCLASS(BlueprintType, Blueprintable)
class TESTPROJECT_API ATPClient : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Network)
		bool CreateClientAndConnect();

	UFUNCTION(BlueprintCallable, Category = Network)
	bool SendTestPacket(const FString userId, const FString password);

protected:
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	bool Initialize();
	bool Connect();
	bool Close();	

	const int HEAD_SIZE = sizeof(PROTOCOL);
	const char* SERVER_IP = "127.0.0.1";
	const uint8_t SERVER_PORT = static_cast<uint8_t>(2738);
	SOCKET hSocket;
	SOCKADDR_IN recvAddr;
	USocketRSThread* rsThread;
};