#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
//#define FD_SETSIZE			100 // 소켓리스트 사이즈(기본값은 64)

#include <iostream>
// For Socket
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;


// 데디케이티드 서버로부터 수신받는 구조체
struct PacketData
{
	char DediIP[16];
	int DediPort;
	int DediPlayerNum;
};



int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_aton()으로 나중에 교체하자
	ListenSockAddr.sin_port = htons(10000);

	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 0);

	SOCKADDR_IN ClientSockAddr;
	int ClientSockAddrLength = sizeof(ClientSockAddr);

	// 소켓리스트용 구조체
	fd_set ReadSocketList;
	fd_set ReadSocketListCopy;

	// fd_set 구조체 초기화. 구조체의 주소를 파라미터로 넣어준다
	FD_ZERO(&ReadSocketList);

	// fd_set 구조체에 넣기. ListenSocket(리스트에 넣을 소켓), &ReadSocketList(리스트)를 파라미터로 넣어준다
	FD_SET(ListenSocket, &ReadSocketList);

	// 몇 초 간격으로 리스트 확인할까? (tv_sec + tv_usec)
	TIMEVAL Timeout;
	Timeout.tv_sec = 0;
	Timeout.tv_usec = 300;

	while (1)
	{
		ReadSocketListCopy = ReadSocketList;
		// select함수는 리스트에서 몇 개가 바뀌었는지를 리턴해준다. 근데 바뀐 것을 표시해준답시고 값을 변경해버리기 때문에 원본이 아닌 복사본을 주고 확인해보라고 해야 한다
		int fd_num = select(0, &ReadSocketListCopy, nullptr, nullptr, &Timeout);
		// Timeout으로 정한 시간보다 오래 걸리는 작업하고 있으면, 그 작업이 아직 안 끝났어도 리스트 확인하나?

		if (fd_num == 0)
		{
			// 다른 일 함

		}
		else if (fd_num == SOCKET_ERROR)
		{
			exit(-1);
		}
		else
		{
			// 리스트에서 바뀐거 찾아라
			for (int i = 0; i < (int)ReadSocketList.fd_count; ++i)
			{
				if (FD_ISSET(ReadSocketList.fd_array[i], &ReadSocketListCopy))
				{
					// accept & recv
					if (ReadSocketList.fd_array[i] == ListenSocket)
					{
						SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
						FD_SET(ClientSocket, &ReadSocketList);

					}
					else
					{
						char Buffer[1024] = { 0, };

						int flag = NULL;
						// 처음에 4바이트만 받아서 int값 확인. 1이면 데디 서버에서 보낸거다.
						int RecvLength = recv(ReadSocketList.fd_array[i], (char*)&Buffer, sizeof(int), 0);
						cout << "FirstRecv" << endl;
						memcpy(&flag, Buffer, sizeof(flag));

						if (RecvLength > 0)
						{
							cout << "Flag: " << flag << endl;

							if (flag == 1)
							{
								PacketData DediData;
								ZeroMemory((void*)&DediData, sizeof(DediData));
								// 이후 데이터 수신
								RecvLength = recv(ReadSocketList.fd_array[i], (char*)&Buffer, sizeof(PacketData), 0);

								//Buffer[RecvLength] = '\0';	//버퍼의 마지막에 null추가

								memcpy(&DediData, Buffer, sizeof(PacketData));
								cout << DediData.DediIP << endl;
								cout << DediData.DediPort << endl;
								cout << DediData.DediPlayerNum << endl;

							}

							/*
							// 받은 메세지 모든 클라이언트에 보내기
							for (int i = 1; i < (int)ReadSocketList.fd_count; i++)
							{
								send(ReadSocketList.fd_array[i], Buffer, sizeof(Buffer), 0);
							}
							*/
						}
						else
						{
							SOCKET ClosedSocket = ReadSocketList.fd_array[i];
							// FD_SET의 반대. 파라미터로 넣은거 없애기. 이거 안 하면 에러난다. 클리어하고 클로즈
							FD_CLR(ClosedSocket, &ReadSocketList);
							closesocket(ClosedSocket);
						}
					}
				}
			}
		}
	}



	closesocket(ListenSocket);


	WSACleanup();

	return 0;
}

