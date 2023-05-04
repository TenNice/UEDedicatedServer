#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
//#define FD_SETSIZE			100 // 소켓리스트 사이즈(기본값은 64)

#include <iostream>

// For DB
#include "mysql/jdbc.h"
#pragma comment(lib, "debug/mysqlcppconn.lib")

// For Socket
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// For strcpy()
#include <cstring>

using namespace std;


// 데디케이티드 서버로부터 수신받는 구조체
struct PacketData
{
	char DediIP[16];
	int DediPort;
	int DediPlayerNum;
	int DediServerState;
};

void SendToDB(sql::Statement* _statement, string IPAddr, int PortNum, int PlayerNum, int DediServerState);
void ReceiveFromDB(sql::Statement* _statement, sql::ResultSet*& _resultset);

int main()
{
	/* DB */
	// 워크벤치 껍데기
	sql::Driver* driver;
	driver = get_driver_instance();

	// 커넥션 껍데기
	sql::Connection* connection;
	connection = driver->connect("192.168.0.15:3306", "sihoon", "qwer1234");
	connection->setSchema("server_info");

	// 쿼리문 날려주는 애 껍데기
	sql::Statement* statement;
	statement = connection->createStatement();

	// 쿼리문 날리고 결과 받을 때 쓰는 애 껍데기
	sql::ResultSet* resultset = nullptr;



	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_aton()으로 나중에 교체하자
	ListenSockAddr.sin_port = htons(10000);

	// Winsock, jdbc 중 어느 bind 사용할건지 명확히 해줘야 한다.
	::bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

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
						memcpy(&flag, Buffer, sizeof(int));

						if (RecvLength > 0)
						{
							cout << "Flag: " << flag << endl;

							// 데디케이티드 서버에서 보낸 패킷
							if (flag == 1)
							{
								PacketData DediData;
								ZeroMemory((void*)&DediData, sizeof(DediData));
								// 이후 데이터 수신
								RecvLength = recv(ReadSocketList.fd_array[i], (char*)&Buffer, sizeof(PacketData), 0);

								// DediData 메모리 공간에 패킷 데이터 그대로 복사
								memcpy(&DediData, Buffer, sizeof(PacketData));
								cout << "IP: " << DediData.DediIP << endl;
								cout << "Port: " << DediData.DediPort << endl;
								cout << "Players: " << DediData.DediPlayerNum << endl;
								cout << "ServerState: " << DediData.DediServerState << endl;

								// Send Dedicated Server Information to DB
								SendToDB(statement, DediData.DediIP, DediData.DediPort, DediData.DediPlayerNum, DediData.DediServerState);

							}
							else if(flag == 2) // 클라이언트에서 보낸 패킷
							{								
								resultset = nullptr;
								PacketData ServerInfo;
								ZeroMemory((void*)&ServerInfo, sizeof(ServerInfo));

								// Receive Dedicated Server List from DB
								ReceiveFromDB(statement, resultset);

								// 플레이어 수가 가장 적은 서버의 IP, Port값
								int MinPlayer = 10000;
								// resultset에 여러개가 들어있으면 이런식으로
								while (resultset->next()) {
									string ip = resultset->getString("ip_address");
									int port = resultset->getInt("port_number");
									int player = resultset->getInt("player_number");
									int state = resultset->getInt("server_state");

									if (player == 0)
									{
										strcpy(ServerInfo.DediIP, ip.c_str());
										ServerInfo.DediPort = port;
										ServerInfo.DediPlayerNum = player;
										ServerInfo.DediServerState = state;
										break;
									}
									else
									{
										if (MinPlayer > player)
										{
											MinPlayer = player;
											strcpy(ServerInfo.DediIP, ip.c_str());
											ServerInfo.DediPort = port;
											ServerInfo.DediPlayerNum = player;
											ServerInfo.DediServerState = state;
										}
									}
								}
								cout << "ip_address: " << ServerInfo.DediIP << ", port_number: " << ServerInfo.DediPort << ", player_number: " << ServerInfo.DediPlayerNum << ", server_state: " << ServerInfo.DediServerState << endl;

								// 버퍼에 구조체 담기
								memcpy(Buffer, &ServerInfo, sizeof(PacketData));

								// 클라이언트한테 패킷 보내기
								int SendLength = send(ReadSocketList.fd_array[i], Buffer, sizeof(Buffer), 0);
							} // end if(flag)

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

	// 만든 것들 역순으로 지우기
	delete resultset;
	delete statement;
	delete connection;


	return 0;
}


void SendToDB(sql::Statement* _statement, string IPAddr, int PortNum, int PlayerNum, int DediServerState)
{
	// DB에 입력될 데이터 값
	string IP = IPAddr;
	string stringPort = to_string(PortNum);
	string stringPlayerNumber = to_string(PlayerNum);
	string stringServerState = to_string(DediServerState);

	// 쿼리문 세팅
	string query = "INSERT INTO server_list (ip_address, port_number, player_number, server_state) VALUES ('" + IP + "', " + stringPort + ", " + stringPlayerNumber + ", " + stringServerState + ") ON DUPLICATE KEY UPDATE player_number=" + stringPlayerNumber + ";";

	try
	{
		// 쿼리문 날리기
		_statement->execute(query);
	}
	catch (sql::SQLException err)
	{
		cout << "Exception : " << __FILE__ << endl;
		cout << "Function : " << __FUNCTION__ << endl;
		cout << "Line : " << __LINE__ << endl;
		cout << "What : " << err.what() << endl;
		cout << "Error code : " << err.getErrorCode() << endl;
	}
}

void ReceiveFromDB(sql::Statement* _statement, sql::ResultSet*& _resultset)
{
	// 쿼리문 세팅
	string query = "SELECT * from server_list";

	try {
		// 실행하고 결과 받아오는게 있을 때 executeQuery 사용하자(결과값은 ResultSet* 타입이다)
		_resultset = _statement->executeQuery(query);
	}
	catch (sql::SQLException err)
	{
		cout << "Exception : " << __FILE__ << endl;
		cout << "Function : " << __FUNCTION__ << endl;
		cout << "Line : " << __LINE__ << endl;
		cout << "What : " << err.what() << endl;
		cout << "Error code : " << err.getErrorCode() << endl;
	}

}
