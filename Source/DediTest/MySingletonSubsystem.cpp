// Fill out your copyright notice in the Description page of Project Settings.


#include "MySingletonSubsystem.h"
#include "Kismet/GameplayStatics.h"


void UMySingletonSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("Initialize GI Subsystem"));

	ServerState = EServerState::None;
	IP = "None";
	Port = NULL;
	PlayerNum = 0;

	// �̷��� �����ν��Ͻ� �ҷ��ͼ� ����ض�! �� �׷��� Fatal Error����. UE������ �°� ����ؾ� �Ѵ�.
	// ����ý��� ���� ����(���߿� ���صǸ� ���) �����ν��Ͻ� or �����ν��Ͻ� ��ӹ��� Ŭ������ ���.
	// �������� ���� �� �ϳ��� ���ô�, �������ϴµ� �ʿ��� ������ ������� �ʴ� ������� �����ϴ� ���. ���ٸ� �ص� ����.
	//Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance())->���Լ�()
}

void UMySingletonSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

//UMySingletonSubsystem* UMySingletonSubsystem::InstanceSingleton = nullptr;
//
//UMySingletonSubsystem* UMySingletonSubsystem::GetInstance()
//{
//	check(InstanceSingleton);
//	return InstanceSingleton;
// 
//}

FString UMySingletonSubsystem::GetIP()
{
	return IP;
}

void UMySingletonSubsystem::SetIP(FString IPAddr)
{
	IP = IPAddr;
}

int32 UMySingletonSubsystem::GetPort()
{
	return Port;
}

void UMySingletonSubsystem::SetPort(int32 PortNum)
{
	Port = PortNum;
}

int32 UMySingletonSubsystem::GetServerState()
{
	return (int32)ServerState;
}

void UMySingletonSubsystem::SetServerState(EServerState State)
{
	ServerState = State;
}

int32 UMySingletonSubsystem::GetPlayerNum()
{
	return PlayerNum;
}

void UMySingletonSubsystem::SetPlayerNum(int32 Number)
{
	PlayerNum = Number;
}
