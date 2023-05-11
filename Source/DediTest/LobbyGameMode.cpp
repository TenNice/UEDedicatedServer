// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LoginActor.h"

// For getting playercontroller
#include "Kismet/GameplayStatics.h"


void ALobbyGameMode::ConnectDedicatedServer(FString DedicatedServerAddress)
{
	// Client Travel API 사용해서 데디케이티드 서버에 접속
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientTravel(DedicatedServerAddress, TRAVEL_Absolute, 0);
	UE_LOG(LogTemp, Warning, TEXT("Success ClientTravel"));
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
		if (MyLoginActor->DoLogin(InputID, InputPW))
		{
			// 로그인 성공했으면 TCP 서버에 접속
			TCPConnect();
		}
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
		// TCP 서버에 접속
		if (MyLoginActor->ConnectTCPServer())
		{
			FString DediServerAddress = MyLoginActor->RequestAddress();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DediServerAddress);

			// 전달 받은 주소로 접속
			ConnectDedicatedServer(DediServerAddress);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed Connecting"));
		}
	}
}