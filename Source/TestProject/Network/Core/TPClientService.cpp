// Fill out your copyright notice in the Description page of Project Settings.

#include "TPClientService.h"
#include "../Packet/PacketProcessor.h"
#include "../Packet/PacketService.h"
#include "../Packet/PacketGeneratorClient.h"
#include "../../Object/ObjUser.h"
#include "../../Core/TPCharacter.h"
#include "../../Util/TPLogger.h"

bool ATPClientService::ReqLogin(const FString& _userId, const FString& _password)
{
	if (isLogined)
	{
		return false;
	}

	TPUtil::GetInstance().WCharToMultiByte(cUserId, SIZE_USER_USER_ID, *_userId);
	TPUtil::GetInstance().WCharToMultiByte(cPassword, SIZE_USER_PASSWORD, *_password);

	auto packet = PacketGeneratorClient::GetInstance().CreateReqLogin(cUserId, cPassword);
	auto result = PacketProcessor::GetInstance().SendPacket(packet);

	if (result)
	{
		propUserId = _userId;
		propPassword = _password;
	}

	return result;
}

bool ATPClientService::ReqMove(const EOpMove operation, const FInputMove& inputMove)
{
	if (!isLogined)
	{
		return false;
	}
		
	FBcastMove bcastMove(propUserId, operation, inputMove);

	auto packet = PacketGeneratorClient::GetInstance().CreateReqMove(bcastMove);
	return PacketProcessor::GetInstance().SendPacket(packet);
}

bool ATPClientService::ReqLocationSync(const FVector& location)
{
	if (!isLogined)
	{
		return false;
	}

	auto packet = PacketGeneratorClient::GetInstance().CreateReqLocationSync(location);
	return PacketProcessor::GetInstance().SendPacket(packet);
}

bool ATPClientService::ReqRoundTripTime()
{
	if (!isLogined)
	{
		return false;
	}

	auto packet = PacketGeneratorClient::GetInstance().CreateReqRoundTripTime();
	return PacketProcessor::GetInstance().SendPacket(packet);
}

bool ATPClientService::ReqAction(const EOpAction operation, const FInputAction& inputAction)
{
	if (!isLogined)
	{
		return false;
	}

	FBcastAction bcastAction(propUserId, operation, inputAction);

	auto packet = PacketGeneratorClient::GetInstance().CreateReqAction(bcastAction);
	return PacketProcessor::GetInstance().SendPacket(packet);
}

bool ATPClientService::ReqAbility(EAbilityType abilityType, const FVector& location, const FVector& rotation)
{
	if (!isLogined)
	{
		return false;
	}

	auto packet = PacketGeneratorClient::GetInstance().CreateReqAbility(abilityType, location, rotation);
	return PacketProcessor::GetInstance().SendPacket(packet);
}

bool ATPClientService::ReqRotationSync(const FVector& rotation)
{
	if (!isLogined)
	{
		return false;
	}

	auto packet = PacketGeneratorClient::GetInstance().CreateReqRotationSync(rotation);
	return PacketProcessor::GetInstance().SendPacket(packet);
}

bool ATPClientService::GetIsLogined() const
{
	return isLogined;
}

int64 ATPClientService::GetRttMs() const
{
	return avgRttMs;
}


void ATPClientService::CallBeginPlay()
{
	SetRecvCallback();
}

void ATPClientService::CallEndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearRecvCallback();
	isLogined = false;
}

void ATPClientService::SetRecvCallback()
{
	PacketService::GetInstance().recvCallError += std::bind(&ATPClientService::CallError, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallResLogin += std::bind(&ATPClientService::CallResLogin, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallResRoundTripTime += std::bind(&ATPClientService::CallResRoundTripTime, this, std::placeholders::_1, std::placeholders::_2);
	PacketService::GetInstance().recvCallBcastEnterGameRoom += std::bind(&ATPClientService::CallBcastEnterGameRoom, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallBcastExitGameRoom += std::bind(&ATPClientService::CallBcastExitGameRoom, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallBcastMove += std::bind(&ATPClientService::CallBcastMove, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallBcastLocationSync += std::bind(&ATPClientService::CallBcastLocationSync, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallBcastAction += std::bind(&ATPClientService::CallBcastAction, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallBcastHit += std::bind(&ATPClientService::CallBcastHit, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallBcastRotationSync += std::bind(&ATPClientService::CallBcastRotationSync, this, std::placeholders::_1);
}

void ATPClientService::ClearRecvCallback()
{
	PacketService::GetInstance().recvCallError.clear();
	PacketService::GetInstance().recvCallResLogin.clear();
	PacketService::GetInstance().recvCallResRoundTripTime.clear();
	PacketService::GetInstance().recvCallBcastEnterGameRoom.clear();
	PacketService::GetInstance().recvCallBcastExitGameRoom.clear();
	PacketService::GetInstance().recvCallBcastMove.clear();
	PacketService::GetInstance().recvCallBcastLocationSync.clear();
	PacketService::GetInstance().recvCallBcastAction.clear();
	PacketService::GetInstance().recvCallBcastHit.clear();
	PacketService::GetInstance().recvCallBcastRotationSync.clear();
}

void ATPClientService::UpdateRtt(const int64 serverTimeMs, const int64 rttMsC2S)
{
	const auto clientTimeMs = TPUtil::GetInstance().TimeSinceEpochMs();
	const auto rttMsS2C = clientTimeMs - serverTimeMs;
	const auto rttMs = rttMsC2S + rttMsS2C;
	
	if (rttMs > MAX_RTT_MS)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("[Ignored] rttMsC2S:%lld rttMsS2C:%lld rttMs:%lld avgRttMs:%lld"), rttMsC2S, rttMsS2C, rttMs, avgRttMs));
		TPLogger::GetInstance().PrintLog("[Ignored] rttMsC2S:%lld rttMsS2C:%lld rttMs:%lld avgRttMs:%lld", rttMsC2S, rttMsS2C, rttMs, avgRttMs);
		return;
	}

	if (rttCount >= MAX_RTT_COUNT)
	{
		totalRttMs = 0;
		rttCount = 0;
	}
	totalRttMs += rttMs;
	rttCount++;
	avgRttMs = totalRttMs / rttCount;

	TPLogger::GetInstance().PrintLog("rttMsC2S:%lld rttMsS2C:%lld rttMs:%lld avgRttMs:%lld", rttMsC2S, rttMsS2C, rttMs, avgRttMs);
}

void ATPClientService::CallError(const FString& message)
{
	K2_RecvCallError(message);
}

void ATPClientService::CallResLogin(const TArray<UObjUser*>& objUserList)
{
	if (!isLogined)
	{
		isLogined = true;
	}
	K2_RecvCallResLogin(objUserList);
}

void ATPClientService::CallResRoundTripTime(const int64 serverTimeMs, const int64 rttMsC2S)
{
	UpdateRtt(serverTimeMs, rttMsC2S);
}

void ATPClientService::CallBcastEnterGameRoom(const UObjUser* const objUser)
{
	K2_RecvCallBcastEnterGameRoom(objUser);
}

void ATPClientService::CallBcastExitGameRoom(const FString& userId)
{
	K2_RecvCallBcastExitGameRoom(userId);
}

void ATPClientService::CallBcastMove(const FBcastMove& bcastMove)
{
	K2_RecvCallBcastMove(bcastMove);
}

void ATPClientService::CallBcastLocationSync(const FBcastLocationSync& bcastLocationSync)
{
	K2_RecvCallBcastLocationSync(bcastLocationSync);
}

void ATPClientService::CallBcastAction(const FBcastAction& bcastAction)
{
	K2_RecvCallBcastAction(bcastAction);
}

void ATPClientService::CallBcastHit(const TArray<UObjUser*>& objUserList)
{
	K2_RecvCallBcastHit(objUserList);
}

void ATPClientService::CallBcastRotationSync(const FBcastRotationSync& bcastRotationSync)
{
	K2_RecvCallBcastRotationSync(bcastRotationSync);
}