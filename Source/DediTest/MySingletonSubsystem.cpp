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

	// 이렇게 게임인스턴스 불러와서 사용해라! 안 그러면 Fatal Error난다. UE구조에 맞게 사용해야 한다.
	// 서브시스템 쓰지 말고(나중에 이해되면 써라) 게임인스턴스 or 게임인스턴스 상속받은 클래스를 써라.
	// 에러나는 이유 중 하나의 예시는, 렌더링하는데 필요한 정보에 권장되지 않는 방법으로 접근하는 경우. 접근만 해도 에러.
	//Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance())->내함수()
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
