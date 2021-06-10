// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreTypes.h"
#include "UObject/Object.h"
#include "HAL/Runnable.h"
#include <WinSock2.h>
#include "Packet.h"
#include "SocketRSThread.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API USocketRSThread : public UObject, public FRunnable
{
	GENERATED_BODY()

public:
	~USocketRSThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	void Start(SOCKET socket);		
	bool SendPacket(const Packet& packet);

	bool IsRunning();
private:
	bool RecvPacket();
	
	const int HEAD_SIZE = sizeof(PROTOCOL);
	SOCKET hSocket;

	bool isStopThread;
	FRunnableThread* thread;
};
