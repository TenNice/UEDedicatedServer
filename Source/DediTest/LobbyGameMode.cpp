// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "Kismet/GameplayStatics.h"


void ALobbyGameMode::TryConnectDedicatedServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Do Client Travel"));

	FString DedicatedServerAddress = FString::Printf(TEXT("%s:%i"), *DedicatedServerIP, DedicatedServerPort);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientTravel(DedicatedServerAddress, TRAVEL_Absolute, 0);

}

void ALobbyGameMode::SetDedicatedServerAddress(FString IP, int32 Port)
{
	DedicatedServerIP = IP;
	DedicatedServerPort = Port;
}

