// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DediTestGameMode.generated.h"

UCLASS(minimalapi)
class ADediTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADediTestGameMode();

	// Dedicated Server File Address and Options
	FString DedicatedServerPath;
	FString DedicatedServerOptions;

	// Run Additional Dedicated Server
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	bool RunDedicatedServer(const FString& Options);

	FString ServerPath;

	// Get URL
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	FString GetDedicatedServerAddress();
};

