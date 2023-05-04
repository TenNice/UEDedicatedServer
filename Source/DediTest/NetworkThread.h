// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameInstance.h"

struct PacketData
{
	int Flag = 1;
	char DediIP[16] = { 0, };
	int DediPort;
	int DediPlayerNum;
	int DediServerState;
};

/**
 * 
 */
class DEDITEST_API NetworkThread : public FRunnable
{
public:
	NetworkThread();
	NetworkThread(TQueue<ServerInfo, EQueueMode::Mpsc>* ServerInfoQueue);
	~NetworkThread();

	bool Init() override;
	uint32 Run() override;
	void Stop() override;

	// Send Packet
	void SendPacketData();

	// Connect TCP Server
	bool ConnectTCPServer();


private:
	class FRunnableThread* Thread;
	class FSocket* Socket;
	
	// To control running thread
	bool bRunThread;
	bool bSend;

	// To check already connected TCP server
	bool bConnectedTCP;

	TQueue<ServerInfo, EQueueMode::Mpsc>* ServerInformationQueue = nullptr;
	PacketData ServerInformations;

};
