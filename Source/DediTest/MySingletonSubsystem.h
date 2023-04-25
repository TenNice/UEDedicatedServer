// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MySingletonSubsystem.generated.h"

enum class EServerState : uint8
{
	None,
	Ready,
	Run,
	Stop
};

/**
 * 
 */
UCLASS()
class DEDITEST_API UMySingletonSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// Start USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	FString GetIP();
	void SetIP(FString IPAddr);

	int32 GetPort();
	void SetPort(int32 PortNum);

	int32 GetServerState();
	void SetServerState(EServerState SetState);

	int32 GetPlayerNum();
	void SetPlayerNum(int32 Num);

private:
	FString IP;
	int32 Port;
	EServerState ServerState;
	int32 PlayerNum;
	
};
