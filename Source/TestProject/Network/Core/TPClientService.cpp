// Fill out your copyright notice in the Description page of Project Settings.

#include "TPClientService.h"
#include "../Packet/PacketProcessor.h"
#include "../Packet/PacketService.h"
#include "../Packet/PacketGenerator.h"
#include "../../Object/ObjUser.h"
#include "../../Core/TPCharacter.h"

bool ATPClientService::ReqLogin(const FString& _userId, const FString& _password)
{
	if (isLogined)
	{
		return false;
	}

	TPUtil::GetInstance().WCharToMultiByte(hUserId, SIZE_USER_USER_ID, *_userId);
	TPUtil::GetInstance().WCharToMultiByte(hPassword, SIZE_USER_PASSWORD, *_password);

	auto packet = PacketGenerator::GetInstance().CreateReqLogin(hUserId, hPassword);
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

	auto packet = PacketGenerator::GetInstance().CreateReqMove(bcastMove);
	return PacketProcessor::GetInstance().SendPacket(packet);
}

bool ATPClientService::ReqMoveSync(const FVector& location)
{
	if (!isLogined)
	{
		return false;
	}

	auto packet = PacketGenerator::GetInstance().CreateReqMoveSync(hUserId, location);
	return PacketProcessor::GetInstance().SendPacket(packet);
}

bool ATPClientService::ReqRoundTripTime()
{
	if (!isLogined)
	{
		return false;
	}

	auto packet = PacketGenerator::GetInstance().CreateReqRoundTripTime();
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
	PacketService::GetInstance().recvCallBcastLocationSync += std::bind(&ATPClientService::CallBcastLocationSync, this, std::placeholders::_1, std::placeholders::_2);
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
}

void ATPClientService::UpdateRtt(const int64 serverTimeMs, const int64 rttMsC2S)
{
	const auto clientTimeMs = TPUtil::GetInstance().TimeSinceEpochMs();
	const auto rttMsS2C = TPUTIL_MAX(clientTimeMs - serverTimeMs, 0);
	const auto rttMs = rttMsC2S + rttMsS2C;
	
	// 왕복 시간이 너무 큰 값이면 무시(1초 기준)
	if (rttMs > MAX_RTT_MS)
	{
		return;
	}

	totalRttMs += rttMs;
	rttCount++;
	avgRttMs = totalRttMs / rttCount;
	UE_LOG(LogTemp, Log, TEXT("rttMsC2S:%lld rttMsS2C:%lld avgRttMs:%lld"), rttMsC2S, rttMsS2C, avgRttMs);
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

void ATPClientService::CallBcastLocationSync(const FString& userId, const FVector& location)
{
	K2_RecvCallBcastLocationSync(userId, location);
}