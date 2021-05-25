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
	bool CreateClientAndConnect(const FString _serverIp, const FString _serverPort);

	UFUNCTION(BlueprintCallable, Category = Network)
	bool ReqLogin(const FString _userId, const FString _password);

protected:		
	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallGameRoomObj", meta = (ScriptName = "RecvCallGameRoomObj"))
	void K2_RecvCallGameRoomObj();		
	void CallGameRoomObj();

	UPROPERTY(BlueprintReadWrite, Category = Network)
	FString propServerIp;

	UPROPERTY(BlueprintReadWrite, Category = Network)
	FString propServerPort;

	UPROPERTY(BlueprintReadWrite, Category = Network)
	FString propUserId;

	UPROPERTY(BlueprintReadWrite, Category = Network)
	FString propPassword;

	UPROPERTY(BlueprintReadOnly, Category = Network)
	bool isConnected;

	UPROPERTY(BlueprintReadOnly, Category = Network)
	bool isLogined;

	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	bool Initialize(const FString _serverIp, const FString _serverPort);
	bool Connect();
	bool Close();	
	void SetRecvCallback();
	void ClearRecvCallback();

	char* serverIp;
	uint8_t serverPort;
	SOCKET hSocket;
	SOCKADDR_IN recvAddr;
	USocketRSThread* rsThread;
};