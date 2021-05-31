// Fill out your copyright notice in the Description page of Project Settings.

#include "TPClientService.h"
#include "PacketProcessor.h"
#include "PacketService.h"
#include "PacketGenerator.h"
#include "ObjUser.h"
#include "TPCharacter.h"

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

bool ATPClientService::ProcessReqMove(const float deltaMs)
{
	if (!isLogined)
	{
		return false;
	}

	auto location = playerCharacter->GetActorLocation();
	if (playerLocationList.Num() == 0)
	{
		playerLocationList.Emplace(location);
		totalDeltaMs = deltaMs;
		return false;
	}
	
	auto last = playerLocationList.Last();
	if (last == location)
	{
		return false;
	}

	playerLocationList.Emplace(location);
	totalDeltaMs += deltaMs;
	
	if (totalDeltaMs >= 0.2f)
	{
		auto packet = PacketGenerator::GetInstance().CreateReqMove(hUserId, playerLocationList);
		auto result = PacketProcessor::GetInstance().SendPacket(packet);
		playerLocationList.Empty();
		totalDeltaMs = 0.f;
		return result;
	}
	return false;
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
	PacketService::GetInstance().recvCallGameRoomObj += std::bind(&ATPClientService::CallGameRoomObj, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallEnterGameRoom += std::bind(&ATPClientService::CallEnterGameRoom, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallExitGameRoom += std::bind(&ATPClientService::CallExitGameRoom, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallMoveLocation += std::bind(&ATPClientService::CallMoveLocation, this, std::placeholders::_1, std::placeholders::_2);
}

void ATPClientService::ClearRecvCallback()
{
	PacketService::GetInstance().recvCallError.clear();
	PacketService::GetInstance().recvCallGameRoomObj.clear();
	PacketService::GetInstance().recvCallEnterGameRoom.clear();
	PacketService::GetInstance().recvCallExitGameRoom.clear();
	PacketService::GetInstance().recvCallMoveLocation.clear();
}

void ATPClientService::CallError(const FString& message)
{
	K2_RecvCallError(message);
}

void ATPClientService::CallGameRoomObj(const TArray<UObjUser*>& objUserList)
{
	if (!isLogined)
	{
		isLogined = true;
	}
	K2_RecvCallGameRoomObj(objUserList);
}

void ATPClientService::CallEnterGameRoom(const UObjUser* const objUser)
{
	K2_RecvCallEnterGameRoom(objUser);
}

void ATPClientService::CallExitGameRoom(const FString& userId)
{
	K2_RecvCallExitGameRoom(userId);
}

void ATPClientService::CallMoveLocation(const FString& userId, const TArray<FVector>& locationList)
{
	K2_RecvCallMoveLocation(userId, locationList);
}