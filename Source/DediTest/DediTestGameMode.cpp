// Copyright Epic Games, Inc. All Rights Reserved.

#include "DediTestGameMode.h"
#include "DediTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

// IP 알아내기용
#include "SocketSubsystem.h"


ADediTestGameMode::ADediTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	ServerPath = FString::Printf(TEXT("C:\\Work\\DediTest\\Packaged\\WindowsServer\\DediTestServer.exe"));

}

bool ADediTestGameMode::RunDedicatedServer(const FString& Options)
{
	// Construct the command line arguments
	FString CommandLine = FString::Printf(TEXT("%s %s -server -log"), *ServerPath, *Options);

	// Run the dedicated server
	bool bSuccess = FPlatformProcess::CreateProc(*ServerPath, *CommandLine, true, false, false, nullptr, 0, nullptr, nullptr).IsValid();
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to run dedicated server"));
	}

	return bSuccess;
}

FString ADediTestGameMode::GetDedicatedServerAddress()
{
	// Get Port Number
	int32 PortNumber = NULL;
	if (GetWorld() != nullptr)
	{
		PortNumber = GetWorld()->URL.Port;
		UE_LOG(LogTemp, Error, TEXT("Port : %d"), PortNumber);
	}

	// Get IP Address
	FString IPAddress = "none";
	bool canBind = false;
	TSharedRef<FInternetAddr>LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);

	if (LocalIp->IsValid())
	{
		IPAddress = LocalIp->ToString(false);
		UE_LOG(LogTemp, Error, TEXT("IP : %s"), *IPAddress);
	}

	// IP + Port
	FString DedicatedServerAddress = FString::Printf(TEXT("%s:%d"), *IPAddress, PortNumber);

	return DedicatedServerAddress;
}

