// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TPClientService.generated.h"

UCLASS(BlueprintType, Blueprintable)
class TESTPROJECT_API ATPClientService : public AActor
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = Network)
	bool ReqLogin(const FString& _userId, const FString& _password);

protected:
	UPROPERTY(BlueprintReadOnly, Category = Network)
	bool isLogined;

	UPROPERTY(BlueprintReadOnly, Category = Network)
	FString propUserId;

	UPROPERTY(BlueprintReadOnly, Category = Network)
	FString propPassword;

	UFUNCTION(BlueprintCallable, Category = TPClientService)
	void CallBeginPlay();

	UFUNCTION(BlueprintCallable, Category = TPClientService)
	void CallEndPlay(const EEndPlayReason::Type EndPlayReason);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallError", meta = (ScriptName = "RecvCallError"))
	void K2_RecvCallError(const FString& message);
	void CallError(const FString& message);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallGameRoomObj", meta = (ScriptName = "RecvCallGameRoomObj"))
	void K2_RecvCallGameRoomObj();
	void CallGameRoomObj();

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallEnterGameRoom", meta = (ScriptName = "RecvCallEnterGameRoom"))
	void K2_RecvCallEnterGameRoom();
	void CallEnterGameRoom();

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallExitGameRoom", meta = (ScriptName = "RecvCallExitGameRoom"))
	void K2_RecvCallExitGameRoom();
	void CallExitGameRoom();

private:
	void SetRecvCallback();
	void ClearRecvCallback();
};
