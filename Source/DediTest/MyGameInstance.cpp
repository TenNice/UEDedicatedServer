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
	//NetworkThread = nullptr;
	MyThreadInstance = nullptr;
}

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("Initialize GI"));
	
}

void UMyGameInstance::CreateThread()
{
	//if (NetworkThread == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Create Thread"));
	//	NetworkThread = new SubThreadForCreateProcess(this);
	//}

	// Create an instance of MyThread
	MyThreadInstance = MakeShareable(new SubThreadForCreateProcess(this));
	// Create a new thread from MyThreadInstance
	//FRunnableThread* Thread = FRunnableThread::Create(MyThreadInstance.Get(), TEXT("MyThread"));
	
}

void UMyGameInstance::DeleteThread()
{
	//if (NetworkThread != nullptr)
	//{
	//	NetworkThread->Stop();
	//	NetworkThread = nullptr;
	//}

	MyThreadInstance->Stop();
}

