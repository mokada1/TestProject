// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TPDefine.h"
#include "TPClientService.generated.h"

class UObjUser;
class ATPCharacter;

UCLASS(BlueprintType, Blueprintable)
class TESTPROJECT_API ATPClientService : public AActor
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = Network)
	bool ReqLogin(const FString& _userId, const FString& _password);

	UFUNCTION(BlueprintCallable, Category = Network)
	bool ProcessReqMove(const float deltaMs);

protected:
	UPROPERTY(BlueprintReadWrite, Category = Network)
	ATPCharacter* playerCharacter;

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
	void K2_RecvCallGameRoomObj(const TArray<UObjUser*>& objUserList);
	void CallGameRoomObj(const TArray<UObjUser*>& objUserList);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallEnterGameRoom", meta = (ScriptName = "RecvCallEnterGameRoom"))
	void K2_RecvCallEnterGameRoom(const UObjUser* const objUser);
	void CallEnterGameRoom(const UObjUser* const objUser);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallExitGameRoom", meta = (ScriptName = "RecvCallExitGameRoom"))
	void K2_RecvCallExitGameRoom(const FString& userId);
	void CallExitGameRoom(const FString& userId);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallMoveLocation", meta = (ScriptName = "RecvCallMoveLocation"))
	void K2_RecvCallMoveLocation(const FString& userId, const TArray<FVector>& locationList);
	void CallMoveLocation(const FString& userId, const TArray<FVector>& locationList);

private:
	void SetRecvCallback();
	void ClearRecvCallback();

	char hUserId[SIZE_USER_USER_ID];
	char hPassword[SIZE_USER_PASSWORD];
	TArray<FVector> playerLocationList;
	float totalDeltaMs;
};
