// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEDITEST_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();

	virtual void Init() override;

	void CreateThread(int32 PortNum);

	// 서버 종료할 때 사용해야 한다
	void DeleteThread();
	
	
private:
	class SubThreadForCreateProcess* ForkProcessThread;

	

};
