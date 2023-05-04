// Copyright Epic Games, Inc. All Rights Reserved.

#include "DediTestGameMode.h"
#include "DediTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

// For getting IP
#include "SocketSubsystem.h"

// For getting gameinstance
#include "Kismet/GameplayStatics.h"

#include "MyGameInstance.h"


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

	// Set PlayerNum ****¹Ì¿Ï¼º****
	Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetPlayerNum(0);

	// Set ServerState
	Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetServerState(1);
}

