// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct PacketData
{
	char DediIP[16];
	int DediPort;
	int DediPlayerNum;
	int DediState;
};

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

	// Connect TCP Server
	bool ConnectTCPServer();

	// 


private:
	class FRunnableThread* Thread;
	class UMySingletonSubsystem* SingletonResource;
	class FSocket* Socket;
	
	// To Control run thread
	bool bRunThread;
	bool bSend;

	PacketData DediData;

	// To check already connected TCP server
	bool bConnectedTCP;
};
