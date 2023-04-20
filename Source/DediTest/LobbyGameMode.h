// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEDITEST_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	void TryConnectDedicatedServer();

	// Dedicated Address
	FString DedicatedServerIP;
	int32 DedicatedServerPort;

	// Address to Destination
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	void SetDedicatedServerAddress(FString IPAddress, int32 PortNumber);

	
};
