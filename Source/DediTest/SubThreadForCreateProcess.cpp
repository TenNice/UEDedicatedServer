// Fill out your copyright notice in the Description page of Project Settings.


#include "SubThreadForCreateProcess.h"

// Singleton class
#include "MySingletonSubsystem.h"


SubThreadForCreateProcess::SubThreadForCreateProcess()
{
	Thread = FRunnableThread::Create(this, TEXT("SubThreadForCreateProcess"));
	SingletonResource = nullptr;

	bRunThread = true;
	bSend = true;
}

SubThreadForCreateProcess::SubThreadForCreateProcess(UGameInstance* GI)
{
	Thread = FRunnableThread::Create(this, TEXT("SubThreadForCreateProcess"));
	if (GI != nullptr)
	{
		SingletonResource = GI->GetSubsystem<UMySingletonSubsystem>();
	}
	
	bRunThread = true;
	bSend = true;
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
	return true;
}

uint32 SubThreadForCreateProcess::Run()
{
	if (SingletonResource != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *SingletonResource->GetIP());
	}

	// Do
	while (bRunThread)
	{
		int32 BytesSent = 0;

		if (bSend)
		{
			//Socket->Send(Buffer, sizeof(Buffer), BytesSent);
			bSend = false;
		}

	}

	return 0;
}

void SubThreadForCreateProcess::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop SubThreadForCreateProcess"));

}

void SubThreadForCreateProcess::SendPacket()
{


}