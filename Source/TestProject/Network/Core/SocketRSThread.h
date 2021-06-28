// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreTypes.h"
#include "UObject/Object.h"
#include "HAL/Runnable.h"
#include <WinSock2.h>
#include "../Packet/Packet.h"
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

	void Start(SOCKET socket, bool _isRecvThread);
	bool SendPacket(const Packet& packet);

	bool IsRunning();
private:
	bool RecvPacket();
	
	SOCKET hSocket;
	bool isStopThread;
	FRunnableThread* thread;
	bool isRecvThread;
};

typedef struct
{
	OVERLAPPED overlapped;
	char buffer[MAX_BUFF_SIZE];
	WSABUF wsaBuf;
	int operation;
} PER_IO_DATA, * LPPER_IO_DATA;