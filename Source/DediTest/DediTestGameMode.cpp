// Copyright Epic Games, Inc. All Rights Reserved.

#include "DediTestGameMode.h"
#include "DediTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

// For getting IP
#include "SocketSubsystem.h"

// For getting gameinstance
#include "Kismet/GameplayStatics.h"

#include "MyGameInstance.h"

// For voice chat
#include "PlayerVoiceChatActor.h"

ADediTestGameMode::ADediTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

void ADediTestGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetResource();

	Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this))->CreateThread();
}

void ADediTestGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Set SpawnActor Parameters
	FVector SpawnLocation = FVector(0.f, 0.f, 0.f);
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;

	// Spawn a new instance of the actor of Voice chat plugin
	AActor* SpawnedActor = GetWorld()->SpawnActor<APlayerVoiceChatActor>(APlayerVoiceChatActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

}


FString ADediTestGameMode::GetDedicatedServerIP()
{
	// Get IP Address
	FString IPAddress = "None";
	bool canBind = false;
	TSharedRef<FInternetAddr>LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);

	if (LocalIp->IsValid())
	{
		IPAddress = LocalIp->ToString(false);
		UE_LOG(LogTemp, Error, TEXT("IP : %s"), *IPAddress);
	}

	return IPAddress;
}

int32 ADediTestGameMode::GetDedicatedServerPort()
{
	// Get Port Number
	int32 PortNumber = NULL;
	if (GetWorld() != nullptr)
	{
		PortNumber = GetWorld()->URL.Port;
		UE_LOG(LogTemp, Error, TEXT("Port : %d"), PortNumber);
	}

	return PortNumber;
}

void ADediTestGameMode::SetResource()
{
	// Set IP
	Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetIP(GetDedicatedServerIP());

	// Set Port
	Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetPort(GetDedicatedServerPort());

	// Set PlayerNum ****수정 필요****
	Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetPlayerNum(0);

	// Set ServerState ****수정 필요****
	Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetServerState(1);
}

