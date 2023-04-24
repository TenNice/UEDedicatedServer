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

	void DeleteThread();
	
	
private:
	class SubThreadForCreateProcess* ForkProcessThread;

	

};
