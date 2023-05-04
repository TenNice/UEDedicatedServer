// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LoginActor.h"

// For getting playercontroller
#include "Kismet/GameplayStatics.h"

// Need Test*****************
void ALobbyGameMode::ConnectDedicatedServer()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientTravel(DedicatedServerAddress, TRAVEL_Absolute, 0);
	UE_LOG(LogTemp, Warning, TEXT("Success ClientTravel"));


	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	//if (PlayerController != nullptr)
	//{
	//	PlayerController->ClientTravel(DedicatedServerAddress, TRAVEL_Absolute);
	//	UE_LOG(LogTemp, Warning, TEXT("Success ClientTravel"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed ClientTravel"));
	//}
}


void ALobbyGameMode::TryRegister()
{
	UE_LOG(LogTemp, Warning, TEXT("TryRegister"));
	if (MyLoginActor == nullptr)
	{
		MyLoginActor = NewObject<ALoginActor>();
	}

	if (MyLoginActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DoRegister"));
		MyLoginActor->DoRegister();
	}

}

void ALobbyGameMode::TryLogin(FString InputID, FString InputPW)
{
	UE_LOG(LogTemp, Warning, TEXT("TryLogin"));
	if (MyLoginActor == nullptr)
	{
		MyLoginActor = NewObject<ALoginActor>();
	}

	if (MyLoginActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DoLogin"));
		MyLoginActor->DoLogin(InputID, InputPW);
	}
}

void ALobbyGameMode::TCPConnect()
{
	if (MyLoginActor == nullptr)
	{
		MyLoginActor = NewObject<ALoginActor>();
	}

	if (MyLoginActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TCP"));
		if (MyLoginActor->ConnectTCPServer())
		{
			DedicatedServerAddress = MyLoginActor->RequestAddress();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DedicatedServerAddress);

			// Need test**************
			//ConnectDedicatedServer();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed Connecting"));
		}
	}
}