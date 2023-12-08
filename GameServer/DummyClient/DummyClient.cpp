#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << "Error : " << errCode << endl;
}

int main()
{
	//윈속 초기화
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 0;
	}
	//소켓 만들기
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}
	//논블로킹 소켓 쓰는법
	u_long on = 1;
	if (ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
	{
		HandleError("ioctSocket");
		return 0;
	}
	//주소 설정
	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);
	//Connect
	while (true)
	{
		if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}
			//이미 Connect된 상태라면
			if (::WSAGetLastError() == WSAEISCONN)
			{
				break;
			}
			//에러
			break;
		}
	}

	cout << "Connected to Server!" << endl;

	char sendBuffer[100] = "Hello World";
	//Send
	while (true)
	{
		if (::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}
			//에러
			break;
		}

		cout << "Send Data Len = " << sizeof(sendBuffer) << endl;

		//Recv
		while (true)
		{
			char recvBuffer[1000];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
				{
					continue;
				}
				//에러
				break;
			}
			else if (recvLen == 0)
			{
				//연결끊김
				break;
			}

			cout << "Recv Data Len = " << recvLen << endl;
			break;
		}

		this_thread::sleep_for(1s);
	}

	//소켓 리소스 반환
	::closesocket(clientSocket);
	//윈속 종료
	::WSACleanup();
}