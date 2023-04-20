// Fill out your copyright notice in the Description page of Project Settings.


#include "SubThreadForCreateProcess.h"

SubThreadForCreateProcess::SubThreadForCreateProcess()
{
	Thread = FRunnableThread::Create(this, TEXT("SubThreadForCreateProcess"));
	Port = 0;
}

SubThreadForCreateProcess::SubThreadForCreateProcess(int32 PortNum)
{
	Thread = FRunnableThread::Create(this, TEXT("SubThreadForCreateProcess")); 
	Port = PortNum;
}

SubThreadForCreateProcess::~SubThreadForCreateProcess()
{
	if (Thread)
	{
		// 실질적으로 쓰레드의 활동을 멈추고 쓰레드를 지우는 곳이다.
		UE_LOG(LogTemp, Warning, TEXT("Delete SubThreadForCreateProcess"));
		Thread->WaitForCompletion();
		Thread->Kill(); // Kill()이 Stop() 호출하네
		delete Thread;
	}
}

bool SubThreadForCreateProcess::Init()
{
	return true;
}

uint32 SubThreadForCreateProcess::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("Run SubThreadForCreateProcess"));

	// 실제 할 일
	

	return 0;
}

void SubThreadForCreateProcess::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop SubThreadForCreateProcess"));

}

void SubThreadForCreateProcess::SendPacket()
{


}