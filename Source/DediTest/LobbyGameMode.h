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
	void ConnectDedicatedServer();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	void TryRegister();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	void TryLogin(FString ID, FString PW);

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
	void TCPConnect();

protected:
	class ALoginActor* MyLoginActor = nullptr;

	FString DedicatedServerAddress = "None";


	
};
