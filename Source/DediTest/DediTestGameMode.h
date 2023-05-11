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

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	// Get IP
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	FString GetDedicatedServerIP();

	// Get Port
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	int32 GetDedicatedServerPort();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	void SetResource();

protected:
};

