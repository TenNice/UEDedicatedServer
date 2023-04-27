// Fill out your copyright notice in the Description page of Project Settings.


#include "SubThreadForCreateProcess.h"

// Singleton class
#include "MySingletonSubsystem.h"

// For Socket Connect
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"


SubThreadForCreateProcess::SubThreadForCreateProcess()
{
	Thread = FRunnableThread::Create(this, TEXT("SubThreadForCreateProcess"));
	SingletonResource = nullptr;

	bRunThread = true;
	bSend = true;
	bConnectedTCP = false;
}

SubThreadForCreateProcess::SubThreadForCreateProcess(UGameInstance* GI)
{
	Thread = FRunnableThread::Create(this, TEXT("SubThreadForCreateProcess"));

	SingletonResource = nullptr;

	if (GI != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%p"), SingletonResource);
		UE_LOG(LogTemp, Warning, TEXT("SingletonResource = GI->GetSubsystem<UMySingletonSubsystem>()"));
		SingletonResource = GI->GetSubsystem<UMySingletonSubsystem>();
		UE_LOG(LogTemp, Warning, TEXT("%p"), SingletonResource);
	}
	
	bRunThread = true;
	bSend = true;
	bConnectedTCP = false;
}

SubThreadForCreateProcess::~SubThreadForCreateProcess()
{
	if (Thread)
	{
		UE_LOG(LogTemp, Warning, TEXT("Delete SubThreadForCreateProcess"));
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

bool SubThreadForCreateProcess::Init()
{
	if (bConnectedTCP)
	{
		bConnectedTCP = ConnectTCPServer();
	}

	return true;
}

uint32 SubThreadForCreateProcess::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("Run"));

	// Do
	SendPacket();
	bSend = false;

	return 0;
}

void SubThreadForCreateProcess::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop SubThreadForCreateProcess"));

}

void SubThreadForCreateProcess::SendPacket()
{
	if (SingletonResource != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *SingletonResource->GetIP());
		UE_LOG(LogTemp, Warning, TEXT("%d"), SingletonResource->GetPort());
		UE_LOG(LogTemp, Warning, TEXT("%d"), SingletonResource->GetPlayerNum());
		UE_LOG(LogTemp, Warning, TEXT("%d"), SingletonResource->GetServerState());

		// Convert FString to array of char
		strncpy_s(DediData.DediIP, sizeof(DediData.DediIP), TCHAR_TO_ANSI(*SingletonResource->GetIP()), _TRUNCATE);
		DediData.DediPort = SingletonResource->GetPort();
		DediData.DediPlayerNum = SingletonResource->GetPlayerNum();
		DediData.DediState = SingletonResource->GetServerState();
	}

	uint8 Buffer[sizeof(DediData)];
	memcpy(Buffer, &DediData, sizeof(DediData));
	int32 BytesSent = 0;
	Socket->Send(Buffer, sizeof(DediData), BytesSent);
}

bool SubThreadForCreateProcess::ConnectTCPServer()
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

