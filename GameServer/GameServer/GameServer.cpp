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
	WSAOVERLAPPED overlapped = {};
};

void CALLBACK RecvCallback(DWORD error, DWORD recvLen, LPWSAOVERLAPPED overlapped, DWORD flags)
{
	cout << "Data Recv Len Callback = " << recvLen << endl;
}

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

	while (true)
	{
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);
		SOCKET clientSocket;
		while (true)
		{
			clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			//잘못된 소켓이면 break
			if (clientSocket != INVALID_SOCKET)
			{
				break;
			}
			//소켓 작업이 완료되지 않음
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}
			//에러 : 문제있는상황
			return 0;
		}

		Session session = Session{ clientSocket };
		WSAEVENT wsaEvent = ::WSACreateEvent();
		session.overlapped.hEvent = wsaEvent;

		cout << "Client Connected !" << endl;

		while (true)
		{
			WSABUF wsaBuf;
			wsaBuf.buf = session.recvBuffer; // 버퍼 시작 주소
			wsaBuf.len = BUFSIZE; // 버퍼 길이

			DWORD recvLen = 0;
			DWORD flags = 0; // 플래그 사용안함
			//비동기 Recv 호출, 마지막 인자에 함수 포인터 넘겨줌
			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, RecvCallback) == SOCKET_ERROR)
			{
				//데이터가 안왔을때 펜딩상태
				//완료 통지 될때까지 대기하다가 완료되면 이벤트 호출
				//Result
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					//Alertable Wait
					::SleepEx(INFINITE, TRUE); //APC를 수행할수 있는 상태로 일시정지
					//::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
				}
				else
				{
					//문제 있는 상황
					break;
				}
			}
			else
			{
				cout << "Data Recv Len = " << recvLen << endl;
			}
		}

		::closesocket(session.socket);
		::WSACloseEvent(wsaEvent);
	}

	// 윈속 종료
	::WSACleanup();
}
                              