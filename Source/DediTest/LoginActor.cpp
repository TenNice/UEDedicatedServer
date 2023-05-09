// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginActor.h"

// For HTTP
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

// For Json
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"

// For Socket Connect
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"


// Sets default values
ALoginActor::ALoginActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALoginActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALoginActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool ALoginActor::DoLogin(FString InputID, FString InputPW)
{
	// Construct the login JSON payload
	TSharedPtr<FJsonObject> LoginJson = MakeShareable(new FJsonObject);
	LoginJson->SetStringField(TEXT("email"), InputID);
	LoginJson->SetStringField(TEXT("password"), InputPW);

	// Convert the JSON payload to a string
	FString RegisterString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&RegisterString);
	FJsonSerializer::Serialize(LoginJson.ToSharedRef(), JsonWriter);

	// URI Base + API
	FString UriBase = TEXT("http://192.168.0.117:8080");
	FString UriLogin = UriBase + TEXT("/api/login");

	// Create the HTTP request and set the content
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(UriLogin);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(RegisterString);

	// Delligate Recieve Response
	Request->OnProcessRequestComplete().BindUObject(this, &ALoginActor::ReceiveLoginResponse);

	// Send the request
	Request->ProcessRequest();

	return bSuccessLogin;
}

// Delligate Recieve Response
void ALoginActor::ReceiveLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	if (bWasSuccessful && Response->GetResponseCode() == 200)
	{
		FString ResponseStr = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("Login response received: %s"), *ResponseStr);
		bSuccessLogin = true;

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login request failed"));
	}

}

void ALoginActor::DoRegister()
{
	// Construct the login JSON payload
	TSharedPtr<FJsonObject> RegisterJson = MakeShareable(new FJsonObject);
	RegisterJson->SetStringField(TEXT("email"), TEXT("TestEmail@naver.com"));
	RegisterJson->SetStringField(TEXT("password"), TEXT("TestPassword"));

	// Convert the JSON payload to a string
	FString RegisterString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&RegisterString);
	FJsonSerializer::Serialize(RegisterJson.ToSharedRef(), JsonWriter);

	// URI Base + API
	FString UriBase = TEXT("http://192.168.0.117:8080");
	FString UriRegister = UriBase + TEXT("/api/signup");

	// Create the HTTP request and set the content
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(UriRegister);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(RegisterString);

	// Send the request
	Request->ProcessRequest();

}

bool ALoginActor::ConnectTCPServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Connect to TCP Server"));

	// Create Socket
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Dedicated Server Socket"));

	// IP를 FString으로 입력받아 저장
	FString TCPServerIP = TEXT("127.0.0.1");
	FIPv4Address IP;
	FIPv4Address::Parse(TCPServerIP, IP);

	int32 Port = 10000;

	// 포트와 소켓을 담는 클래스
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(Port);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));
	}

	// 연결 시도. 결과를 받아옴
	bool bIsConnected = Socket->Connect(*Addr);

	return bIsConnected;
}

// 접속된 TCP 서버에 데디케이티드 서버 주소 요청
FString ALoginActor::RequestAddress()
{
	// Send
	int Flag = 2;
	uint8 Buffer[1024] = { 0, };
	memcpy(Buffer, &Flag, sizeof(int));
	int32 BytesSent = 0;
	Socket->Send(Buffer, sizeof(int), BytesSent);

	// Recv 하기 전 초기화
	memset(DestinationInfo.DediIP, 0, sizeof(char) * 16);
	DestinationInfo.DediPort = NULL;
	DestinationInfo.DediPlayerNum = NULL;
	DestinationInfo.DediServerState = NULL;

	// Receive
	int32 BytesRecv = 0;
	Socket->Recv(Buffer, sizeof(ReceivedPacketData), BytesRecv);
	memcpy((void*)&DestinationInfo, &Buffer, sizeof(ReceivedPacketData));

	//FString IP = FString::Printf(TEXT("%d"), DestinationInfo.DediIP); // 이건 이상하게 바뀐다. 왜?
	// FString IP()로 하면 FString 클래스의 생성자 이용하는거다. char[]을 넣어줘서 string으로 변경
	FString IP(DestinationInfo.DediIP);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *IP);
	UE_LOG(LogTemp, Warning, TEXT("%u"), DestinationInfo.DediPort);
	UE_LOG(LogTemp, Warning, TEXT("%u"), DestinationInfo.DediPlayerNum);
	UE_LOG(LogTemp, Warning, TEXT("%u"), DestinationInfo.DediServerState);


	return FString::Printf(TEXT("%s:%d"), *IP, DestinationInfo.DediPort);
}
