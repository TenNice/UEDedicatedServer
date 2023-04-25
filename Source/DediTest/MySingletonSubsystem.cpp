// Fill out your copyright notice in the Description page of Project Settings.


#include "MySingletonSubsystem.h"


void UMySingletonSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ServerState = EServerState::None;
	IP = nullptr;
	Port = NULL;
	PlayerNum = 0;
}

void UMySingletonSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FString UMySingletonSubsystem::GetIP()
{
	return IP;
}

void UMySingletonSubsystem::SetIP(FString IPAddr)
{
	IP = IPAddr;
}

int32 UMySingletonSubsystem::GetPort()
{
	return Port;
}

void UMySingletonSubsystem::SetPort(int32 PortNum)
{
	Port = PortNum;
}

int32 UMySingletonSubsystem::GetServerState()
{
	return (int32)ServerState;
}

void UMySingletonSubsystem::SetServerState(EServerState State)
{
	ServerState = State;
}

int32 UMySingletonSubsystem::GetPlayerNum()
{
	return PlayerNum;
}

void UMySingletonSubsystem::SetPlayerNum(int32 Number)
{
	PlayerNum = Number;
}
