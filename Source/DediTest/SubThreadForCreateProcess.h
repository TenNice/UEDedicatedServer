// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DEDITEST_API SubThreadForCreateProcess : public FRunnable
{
public:
	SubThreadForCreateProcess();
	SubThreadForCreateProcess(UGameInstance* GI);
	~SubThreadForCreateProcess();

	bool Init() override;
	uint32 Run() override;
	void Stop() override;

	// Send Packet
	void SendPacket();


private:
	class FRunnableThread* Thread;
	class UMySingletonSubsystem* SingletonResource;
	
	// To Control run thread
	bool bRunThread;
	bool bSend;
};
