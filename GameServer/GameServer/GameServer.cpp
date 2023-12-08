#include <iostream>

#include "pch.h"
#include "CorePch.h"
#include <thread>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << "Error : " << errCode << endl;
}

const int32 BUFSIZE = 1000;

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
	int32 sendBytes = 0;
};

int main()
{
	// 윈속 초기화
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;
	//소켓 생성
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}
	//논블로킹 소켓 쓰는법
	u_long on = 1;
	if (ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
	{
		HandleError("ioctSocket");
		return 0;
	}
	//주소 설정
	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);
	//바인드
	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		return 0;
	}

	if (::listen(listenSocket, SOMAXCONN))
	{
		return 0;
	}

	cout << "Accept" << endl;

	// Select 모델
	vector<Session> sessions;
	sessions.reserve(100);

	fd_set reads;
	fd_set writes;

	while (true)
	{
		// 소켓 set 초기화
		FD_ZERO(&reads);
		FD_ZERO(&writes);

		//ListenSocket 등록
		FD_SET(listenSocket, &reads);

		//소켓 등록
		for (Session& s : sessions)
		{
			if (s.recvBytes <= s.sendBytes)
			{
				FD_SET(s.socket, &reads);
			}
			else
			{
				FD_SET(s.socket, &writes);
			}
		}
		// 마지막 인자 timeout 설정가능 timeval 구조체 (대기시간)
		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
		if (retVal == SOCKET_ERROR)
		{
			break;
		}

		//Listener 소켓 체크
		if (FD_ISSET(listenSocket, &reads))
		{
			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;
				sessions.push_back(Session{ clientSocket });
			}
		}

		//나머지 소켓 체크
		for (Session& s : sessions)
		{
			//Read
			if (FD_ISSET(s.socket, &reads))
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen <= 0)
				{
					//sessions 제거
					continue;
				}

				s.recvBytes = recvLen;
			}
			//Write
			if (FD_ISSET(s.socket, &writes))
			{
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR)
				{
					continue;
				}
				//데이터의 일부를 보냄
				s.sendBytes += sendLen;
				//데이터가 모두 보내짐
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}
			}
		}
	}

	// 윈속 종료
	::WSACleanup();
}
                              