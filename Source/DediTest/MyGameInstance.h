// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

struct ServerInfo
{
	FString IP;
	int32 Port;
	int32 PlayerNum;
};


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

	UFUNCTION(BlueprintCallable, meta = (KeyWords = "Sihoon"))
	void CreateThread();

	void DeleteThread();

	// Get, Set Data
	FString GetIP();
	void SetIP(FString IPAddr);

	int32 GetPort();
	void SetPort(int32 PortNum);

	int32 GetPlayerNum();
	void SetPlayerNum(int32 Num);
	// End Get, Set Data

	TQueue<ServerInfo, EQueueMode::Mpsc> MessageQueue;
	
private:
	class NetworkThread* _NetworkThread;

	//TSharedPtr<class SubThreadForCreateProcess> MyThreadInstance;

	ServerInfo ServerInformation;
};
