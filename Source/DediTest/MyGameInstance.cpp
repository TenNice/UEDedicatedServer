// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "NetworkThread.h"

// For GetURL
#include "Engine.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#include "DediTestGameMode.h"

UMyGameInstance::UMyGameInstance()
{
	_NetworkThread = nullptr;
}

void UMyGameInstance::Init()
{
	Super::Init();
}

void UMyGameInstance::CreateThread()
{
	// 메세지 큐 테스트
	//ServerInformation.IP = "123.123.0.1";
	//ServerInformation.Port = 9999;
	//ServerInformation.PlayerNum = 5;

	UE_LOG(LogTemp, Warning, TEXT("Create Thread"));

	MessageQueue.Enqueue(ServerInformation);

	if (_NetworkThread == nullptr)
	{
		_NetworkThread = new NetworkThread(&MessageQueue);
	}
}

void UMyGameInstance::DeleteThread()
{
	if (_NetworkThread != nullptr)
	{
		_NetworkThread->Stop();
		_NetworkThread = nullptr;
	}

}

FString UMyGameInstance::GetIP()
{
	return ServerInformation.IP;
}

void UMyGameInstance::SetIP(FString IPAddr)
{
	ServerInformation.IP = IPAddr;
}

int32 UMyGameInstance::GetPort()
{
	return ServerInformation.Port;
}

void UMyGameInstance::SetPort(int32 PortNum)
{
	ServerInformation.Port = PortNum;
}

int32 UMyGameInstance::GetPlayerNum()
{
	return ServerInformation.PlayerNum;
}

void UMyGameInstance::SetPlayerNum(int32 Number)
{
	ServerInformation.PlayerNum = Number;
}

int32 UMyGameInstance::GetServerState()
{
	return ServerInformation.ServerState;
}

void UMyGameInstance::SetServerState(int32 State)
{
	ServerInformation.ServerState = State;
}

