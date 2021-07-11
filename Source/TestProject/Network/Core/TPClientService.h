// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../../Util/TPDefine.h"
#include "../Struct/BcastMove.h"
#include "../Struct/BcastAction.h"
#include "../Struct/BcastRotationSync.h"
#include "../Struct/BcastLocationSync.h"
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
	bool ReqMove(const EOpMove operation, const FInputMove& inputMove);

	UFUNCTION(BlueprintCallable, Category = Network)
	bool ReqLocationSync(const FVector& location);

	UFUNCTION(BlueprintCallable, Category = Network)
	bool ReqRoundTripTime();

	UFUNCTION(BlueprintCallable, Category = Network)
	bool ReqAction(const EOpAction operation, const FInputAction& inputAction);

	UFUNCTION(BlueprintCallable, Category = Network)
	bool ReqDamage();

	UFUNCTION(BlueprintCallable, Category = Network)
	bool ReqRotationSync(const FVector& rotation);

	UFUNCTION(BlueprintCallable, Category = Network)
	bool GetIsLogined() const;

	UFUNCTION(BlueprintCallable, Category = Network)
	int64 GetRttMs() const;

protected:
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

	void CallResRoundTripTime(const int64 serverTimeMs, const int64 rttMsC2S);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastEnterGameRoom", meta = (ScriptName = "RecvCallBcastEnterGameRoom"))
	void K2_RecvCallBcastEnterGameRoom(const UObjUser* const objUser);
	void CallBcastEnterGameRoom(const UObjUser* const objUser);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastExitGameRoom", meta = (ScriptName = "RecvCallBcastExitGameRoom"))
	void K2_RecvCallBcastExitGameRoom(const FString& userId);
	void CallBcastExitGameRoom(const FString& userId);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastMove", meta = (ScriptName = "RecvCallBcastMove"))
	void K2_RecvCallBcastMove(const FBcastMove& bcastMove);
	void CallBcastMove(const FBcastMove& bcastMove);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastLocationSync", meta = (ScriptName = "RecvCallBcastLocationSync"))
	void K2_RecvCallBcastLocationSync(const FBcastLocationSync& bcastLocationSync);
	void CallBcastLocationSync(const FBcastLocationSync& bcastLocationSync);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastAction", meta = (ScriptName = "RecvCallBcastAction"))
	void K2_RecvCallBcastAction(const FBcastAction& bcastAction);
	void CallBcastAction(const FBcastAction& bcastAction);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastHit", meta = (ScriptName = "RecvCallBcastHit"))
	void K2_RecvCallBcastHit(const FString& userId);
	void CallBcastHit(const FString& userId);

	UFUNCTION(BlueprintImplementableEvent, Category = Network, DisplayName = "RecvCallBcastRotationSync", meta = (ScriptName = "RecvCallBcastRotationSync"))
	void K2_RecvCallBcastRotationSync(const FBcastRotationSync& bcastRotationSync);
	void CallBcastRotationSync(const FBcastRotationSync& bcastRotationSync);

	UPROPERTY(BlueprintReadOnly, Category = Network)
	FString propUserId;

	UPROPERTY(BlueprintReadOnly, Category = Network)
	FString propPassword;

private:
	void SetRecvCallback();
	void ClearRecvCallback();
	void UpdateRtt(const int64_t serverTimeMs, const int64_t rttMsC2S);

	bool isLogined;

	char cUserId[SIZE_USER_USER_ID];
	char cPassword[SIZE_USER_PASSWORD];

	int64 totalRttMs;
	int64 rttCount;
	int64 avgRttMs;
};
