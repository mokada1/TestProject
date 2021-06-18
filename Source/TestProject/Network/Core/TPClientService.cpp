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
	auto result = PacketProcessor::GetInstance().SendPacket(packet);

	return result;
}

bool ATPClientService::ReqMoveSync(const FVector& location)
{
	if (!isLogined)
	{
		return false;
	}

	auto packet = PacketGenerator::GetInstance().CreateReqMoveSync(hUserId, location);
	auto result = PacketProcessor::GetInstance().SendPacket(packet);

	return result;
}

bool ATPClientService::GetIsLogined() const
{
	return isLogined;
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
	PacketService::GetInstance().recvCallBcastEnterGameRoom += std::bind(&ATPClientService::CallBcastEnterGameRoom, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallBcastExitGameRoom += std::bind(&ATPClientService::CallBcastExitGameRoom, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallBcastMove += std::bind(&ATPClientService::CallBcastMove, this, std::placeholders::_1);
	PacketService::GetInstance().recvCallBcastLocationSync += std::bind(&ATPClientService::CallBcastLocationSync, this, std::placeholders::_1, std::placeholders::_2);
}

void ATPClientService::ClearRecvCallback()
{
	PacketService::GetInstance().recvCallError.clear();
	PacketService::GetInstance().recvCallResLogin.clear();
	PacketService::GetInstance().recvCallBcastEnterGameRoom.clear();
	PacketService::GetInstance().recvCallBcastExitGameRoom.clear();
	PacketService::GetInstance().recvCallBcastMove.clear();
	PacketService::GetInstance().recvCallBcastLocationSync.clear();
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