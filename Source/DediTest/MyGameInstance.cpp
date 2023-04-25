// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "SubThreadForCreateProcess.h"

// For GetURL
#include "Engine.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#include "DediTestGameMode.h"

UMyGameInstance::UMyGameInstance()
{
	NetworkThread = nullptr;
}

void UMyGameInstance::Init()
{
	Super::Init();
	
}

void UMyGameInstance::CreateThread()
{
	NetworkThread = new SubThreadForCreateProcess(this);
}

void UMyGameInstance::DeleteThread()
{
	if (NetworkThread != nullptr)
	{
		NetworkThread->Stop();
		NetworkThread = nullptr;
	}

}

