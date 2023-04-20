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

	// �Ʒ� 3���� �Լ��� ���� ȣ������ �ʰ� �ڵ������� ������ ���̴�.
	// �����带 �Ҵ��� �Լ�
	bool Init() override;
	// �����带 Ȱ���ؼ� �� ���� ���� �Լ�
	uint32 Run() override;
	// �����尡 ���� ����Ǿ�� �ϴ��� �� �� ���, Run()���� ȣ���Ѵ�.
	void Stop() override;

	// Send Packet
	void SendPacket();


private:
	class FRunnableThread* Thread;
	int32 Port;

	
};
