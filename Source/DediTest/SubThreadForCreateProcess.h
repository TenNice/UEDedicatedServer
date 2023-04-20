// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DEDITEST_API SubThreadForCreateProcess : public FRunnable
{
public:
	SubThreadForCreateProcess();
	SubThreadForCreateProcess(int32 PortNum);
	~SubThreadForCreateProcess();

	// 아래 3개의 함수는 따로 호출하지 않고 자동적으로 동작할 것이다.
	// 쓰레드를 할당할 함수
	bool Init() override;
	// 쓰레드를 활용해서 할 일을 넣을 함수
	uint32 Run() override;
	// 쓰레드가 언제 종료되어야 하는지 알 때 사용, Run()에서 호출한다.
	void Stop() override;

	// Send Packet
	void SendPacket();


private:
	class FRunnableThread* Thread;
	int32 Port;

	
};
