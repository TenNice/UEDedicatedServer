// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

//#include "Http.h"
#include "HttpModule.h"

#include "LoginActor.generated.h"


struct ReceivedPacketData
{
	char DediIP[16];
	int DediPort;
	int DediPlayerNum;
	int DediServerState;
};

UCLASS()
class DEDITEST_API ALoginActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoginActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class FSocket* Socket;

	bool bSuccessConnect = false;

	FString DestinaionIP;
	int32 DestinationPort;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DoLogin(FString InputID, FString InputPW);

	void ReceiveLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Sihoon"))
		void DoRegister();

	// TCP Server
	bool ConnectTCPServer();

	FString RequestAddress();

	ReceivedPacketData DestinationInfo;

};
