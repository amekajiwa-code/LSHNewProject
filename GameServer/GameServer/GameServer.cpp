#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

const int32 BUFSIZE = 1000;

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
	WSAOVERLAPPED recvOverlapped = {};
	WSAOVERLAPPED sendOverlapped = {};
};

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	vector<Session*> sessionManager;

	while (true)
	{
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket;
		while (true)
		{
			clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
				break;

			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// 문제 있는 상황
			return 0;
		}

		Session* session = new Session;
		session->socket = clientSocket;
		WSAEVENT wsaEvent = ::WSACreateEvent();
		session->recvOverlapped.hEvent = wsaEvent;
		session->sendOverlapped.hEvent = wsaEvent;
		sessionManager.push_back(session);

		cout << "Client Connected !" << endl;

		while (true)
		{
			//Recv
			WSABUF wsaBuf;
			wsaBuf.buf = session->recvBuffer;
			wsaBuf.len = BUFSIZE;

			DWORD recvLen = 0;
			DWORD flags = 0;
			
			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session->recvOverlapped, nullptr) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Pending
					::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
					::WSAGetOverlappedResult(session->socket, &session->recvOverlapped, &recvLen, FALSE, &flags);
				}
				else
				{
					// TODO : 문제 있는 상황
					break;
				}
			}

			cout << "Data Recv Len = " << recvLen << endl;

			//Send
			WSABUF sendWsaBuf;
			sendWsaBuf.buf = session->recvBuffer;
			sendWsaBuf.len = size(session->recvBuffer);

			DWORD sendLen = 0;
			DWORD sendFlags = 0;
			if (::WSASend(clientSocket, &sendWsaBuf, 1, &sendLen, sendFlags, &session->sendOverlapped, nullptr) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Pending
					::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
					::WSAGetOverlappedResult(clientSocket, &session->sendOverlapped, &sendLen, FALSE, &sendFlags);
				}
				else
				{
					// 진짜 문제 있는 상황
					cout << "진짜 문제 있는 상황" << endl;
					break;
				}
			}

			cout << "Send Data Len! = " << sizeof(session->recvBuffer) << endl;
		}

		::closesocket(session->socket);
		::WSACloseEvent(wsaEvent);
	}

	// 윈속 종료
	::WSACleanup();
}