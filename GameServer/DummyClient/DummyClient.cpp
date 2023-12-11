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
	//이벤트 생성
	WSAEVENT wsaEvent = ::WSACreateEvent();
	WSAOVERLAPPED overlapped = {};
	overlapped.hEvent = wsaEvent;

	//Send : WSA 비동기
	while (true)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = sendBuffer; // 버퍼 시작 주소
		wsaBuf.len = 100; // 버퍼 길이

		DWORD sendLen = 0;
		DWORD flags = 0; // 플래그 사용안함
		if (::WSASend(clientSocket, &wsaBuf, 1, &sendLen, flags, &overlapped, nullptr) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSA_IO_PENDING)
			{
				::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
				::WSAGetOverlappedResult(clientSocket, &overlapped, &sendLen, FALSE, &flags);
			}
			else
			{
				//문제 있는 상황
				break;
			}
		}

		cout << "Send Data Len = " << sizeof(sendBuffer) << endl;

		this_thread::sleep_for(1s);
	}

	//소켓 리소스 반환
	::closesocket(clientSocket);
	::WSACloseEvent(wsaEvent);
	//윈속 종료
	::WSACleanup();
}