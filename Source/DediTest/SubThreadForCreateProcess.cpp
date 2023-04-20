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
		// ���������� �������� Ȱ���� ���߰� �����带 ����� ���̴�.
		UE_LOG(LogTemp, Warning, TEXT("Delete SubThreadForCreateProcess"));
		Thread->WaitForCompletion();
		Thread->Kill(); // Kill()�� Stop() ȣ���ϳ�
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

	// ���� �� ��
	

	return 0;
}

void SubThreadForCreateProcess::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop SubThreadForCreateProcess"));

}

void SubThreadForCreateProcess::SendPacket()
{


}