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

	// �������ݿ�. 1�̸� ���� �������� ������ ��Ŷ�̴�.
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

	// IP�� FString���� �Է¹޾� ����
	FString TCPServerIP = TEXT("127.0.0.1");
	FIPv4Address IP;
	FIPv4Address::Parse(TCPServerIP, IP);

	int32 Port = 10000;

	// ��Ʈ�� ������ ��� Ŭ����
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(Port);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));
	}

	// ���� �õ�. ����� �޾ƿ�
	bool bIsConnetcted = Socket->Connect(*Addr);

	return bIsConnetcted;
}

