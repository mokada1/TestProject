// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../Util/TPDefine.h"
#include "Struct/BcastMove.h"
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
	bool ProcessReqMove(const EOpMove operation, const FInputMove& inputMove);

	UFUNCTION(BlueprintCallable, Category = Network)
	bool GetIsLogined() const;

protected:
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

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallResLogin", meta = (ScriptName = "RecvCallResLogin"))
	void K2_RecvCallResLogin(const TArray<UObjUser*>& objUserList);
	void CallResLogin(const TArray<UObjUser*>& objUserList);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastEnterGameRoom", meta = (ScriptName = "RecvCallBcastEnterGameRoom"))
	void K2_RecvCallBcastEnterGameRoom(const UObjUser* const objUser);
	void CallBcastEnterGameRoom(const UObjUser* const objUser);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastExitGameRoom", meta = (ScriptName = "RecvCallBcastExitGameRoom"))
	void K2_RecvCallBcastExitGameRoom(const FString& userId);
	void CallBcastExitGameRoom(const FString& userId);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastMove", meta = (ScriptName = "RecvCallBcastMove"))
	void K2_RecvCallBcastMove(const FBcastMove& bcastMove);
	void CallBcastMove(const FBcastMove& bcastMove);

private:
	void SetRecvCallback();
	void ClearRecvCallback();

	bool isLogined;

	char hUserId[SIZE_USER_USER_ID];
	char hPassword[SIZE_USER_PASSWORD];
};
