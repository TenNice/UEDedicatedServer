// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LoginActor.h"

// For getting playercontroller
#include "Kismet/GameplayStatics.h"


void ALobbyGameMode::ConnectDedicatedServer(FString DedicatedServerAddress)
{
	// Client Travel API ����ؼ� ��������Ƽ�� ������ ����
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
			// �α��� ���������� TCP ������ ����
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
		// TCP ������ ����
		if (MyLoginActor->ConnectTCPServer())
		{
			FString DediServerAddress = MyLoginActor->RequestAddress();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DediServerAddress);

			// ���� ���� �ּҷ� ����
			ConnectDedicatedServer(DediServerAddress);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed Connecting"));
		}
	}
}