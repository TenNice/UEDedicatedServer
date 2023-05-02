#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
//#define FD_SETSIZE			100 // ���ϸ���Ʈ ������(�⺻���� 64)

#include <iostream>
// For Socket
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;


// ��������Ƽ�� �����κ��� ���Ź޴� ����ü
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
	ListenSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_aton()���� ���߿� ��ü����
	ListenSockAddr.sin_port = htons(10000);

	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 0);

	SOCKADDR_IN ClientSockAddr;
	int ClientSockAddrLength = sizeof(ClientSockAddr);

	// ���ϸ���Ʈ�� ����ü
	fd_set ReadSocketList;
	fd_set ReadSocketListCopy;

	// fd_set ����ü �ʱ�ȭ. ����ü�� �ּҸ� �Ķ���ͷ� �־��ش�
	FD_ZERO(&ReadSocketList);

	// fd_set ����ü�� �ֱ�. ListenSocket(����Ʈ�� ���� ����), &ReadSocketList(����Ʈ)�� �Ķ���ͷ� �־��ش�
	FD_SET(ListenSocket, &ReadSocketList);

	// �� �� �������� ����Ʈ Ȯ���ұ�? (tv_sec + tv_usec)
	TIMEVAL Timeout;
	Timeout.tv_sec = 0;
	Timeout.tv_usec = 300;

	while (1)
	{
		ReadSocketListCopy = ReadSocketList;
		// select�Լ��� ����Ʈ���� �� ���� �ٲ�������� �������ش�. �ٵ� �ٲ� ���� ǥ�����ش�ð� ���� �����ع����� ������ ������ �ƴ� ���纻�� �ְ� Ȯ���غ���� �ؾ� �Ѵ�
		int fd_num = select(0, &ReadSocketListCopy, nullptr, nullptr, &Timeout);
		// Timeout���� ���� �ð����� ���� �ɸ��� �۾��ϰ� ������, �� �۾��� ���� �� ����� ����Ʈ Ȯ���ϳ�?

		if (fd_num == 0)
		{
			// �ٸ� �� ��

		}
		else if (fd_num == SOCKET_ERROR)
		{
			exit(-1);
		}
		else
		{
			// ����Ʈ���� �ٲ�� ã�ƶ�
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
						// ó���� 4����Ʈ�� �޾Ƽ� int�� Ȯ��. 1�̸� ���� �������� �����Ŵ�.
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
								// ���� ������ ����
								RecvLength = recv(ReadSocketList.fd_array[i], (char*)&Buffer, sizeof(PacketData), 0);

								//Buffer[RecvLength] = '\0';	//������ �������� null�߰�

								memcpy(&DediData, Buffer, sizeof(PacketData));
								cout << DediData.DediIP << endl;
								cout << DediData.DediPort << endl;
								cout << DediData.DediPlayerNum << endl;

							}

							/*
							// ���� �޼��� ��� Ŭ���̾�Ʈ�� ������
							for (int i = 1; i < (int)ReadSocketList.fd_count; i++)
							{
								send(ReadSocketList.fd_array[i], Buffer, sizeof(Buffer), 0);
							}
							*/
						}
						else
						{
							SOCKET ClosedSocket = ReadSocketList.fd_array[i];
							// FD_SET�� �ݴ�. �Ķ���ͷ� ������ ���ֱ�. �̰� �� �ϸ� ��������. Ŭ�����ϰ� Ŭ����
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

