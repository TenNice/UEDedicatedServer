// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkThread.h"

// For Socket Connect
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"

// For getting gameinstance
#include "Kismet/GameplayStatics.h"



NetworkThread::NetworkThread()
{
	Thread = FRunnableThread::Create(this, TEXT("SubThreadForCreateProcess"));

	bRunThread = true;
	bSend = true;
	bConnectedTCP = true;

	// 프로토콜용. 1이면 데디 서버에서 보내는 패킷이다.
	//ServerInformations.Flag = 1;
	
}

NetworkThread::NetworkThread(TQueue<ServerInfo, EQueueMode::Mpsc>* ServerInfoQueue)
{
	Thread = FRunnableThread::Create(this, TEXT("SubThreadForCreateProcess"));

	bRunThread = true;
	bSend = true;
	bConnectedTCP = true;

	ServerInformationQueue = ServerInfoQueue;
}

NetworkThread::~NetworkThread()
{
	if (Thread)
	{
		UE_LOG(LogTemp, Warning, TEXT("Delete SubThreadForCreateProcess"));
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

bool NetworkThread::Init()
{
	return true;
}

uint32 NetworkThread::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("Run"));

	while (bConnectedTCP)
	{
		bConnectedTCP = !ConnectTCPServer();
	}

	// Do
	SendPacketData();
	bSend = false;

	return 0;
}

void NetworkThread::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop SubThreadForCreateProcess"));

}

void NetworkThread::SendPacketData()
{
	ServerInfo ReceivedData;
	while (ServerInformationQueue->Dequeue(ReceivedData))
	{
		// Convert FString to array of char
		strncpy_s(ServerInformations.DediIP, sizeof(ServerInformations.DediIP), TCHAR_TO_ANSI(*ReceivedData.IP), _TRUNCATE);
		ServerInformations.DediPort = ReceivedData.Port;
		ServerInformations.DediPlayerNum = ReceivedData.PlayerNum;
	}
	UE_LOG(LogTemp, Warning, TEXT("%c"), *ServerInformations.DediIP);
	UE_LOG(LogTemp, Warning, TEXT("%u"), ServerInformations.DediPort);
	UE_LOG(LogTemp, Warning, TEXT("%u"), ServerInformations.DediPlayerNum);


	uint8 Buffer[sizeof(ServerInformations)];
	memcpy(Buffer, &ServerInformations, sizeof(ServerInformations));
	int32 BytesSent = 0;
	Socket->Send(Buffer, sizeof(ServerInformations), BytesSent);
}

bool NetworkThread::ConnectTCPServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Connect to TCP Server"));

	// Create Socket
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	// IP를 FString으로 입력받아 저장
	FString TCPServerIP = TEXT("127.0.0.1");
	FIPv4Address IP;
	FIPv4Address::Parse(TCPServerIP, IP);

	int32 Port = 10000;

	// 포트와 소켓을 담는 클래스
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(Port);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));
	}

	// 연결 시도. 결과를 받아옴
	bool bIsConnetcted = Socket->Connect(*Addr);

	return bIsConnetcted;
}

