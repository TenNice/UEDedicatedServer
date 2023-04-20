// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "SubThreadForCreateProcess.h"

// For GetURL
#include "Engine.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

UMyGameInstance::UMyGameInstance()
{
	ForkProcessThread = nullptr;
}

void UMyGameInstance::Init()
{
	Super::Init();

}

void UMyGameInstance::CreateThread(int32 PortNumber)
{
	if (ForkProcessThread == nullptr)
	{
		ForkProcessThread = new SubThreadForCreateProcess(PortNumber);
	}

}

void UMyGameInstance::DeleteThread()
{
	// FRunnable API를 이용한 쓰레드는 run함수가 끝나면 알아서 사라진다
	if (ForkProcessThread != nullptr)
	{
		ForkProcessThread->Stop();
		ForkProcessThread = nullptr;
	}

}

