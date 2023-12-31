﻿#include "pch.h"
//#include <iostream>
//#include "CorePch.h"
//#include <atomic>
//#include <mutex>
//#include <windows.h>
//#include <future>
//#include "ThreadManager.h"
//
//#include <winsock2.h>
//#include <mswsock.h>
//#include <ws2tcpip.h>
//#pragma comment(lib, "ws2_32.lib")
//
//#include "Memory.h"
//
//void HandleError(const char* cause)
//{
//	int32 errCode = ::WSAGetLastError();
//	cout << cause << " ErrorCode : " << errCode << endl;
//}
//
//const int32 BUFSIZE = 1000;
//int32 Session_Count = 0;
//
//struct Session
//{
//	SOCKET socket = INVALID_SOCKET;
//	char recvBuffer[BUFSIZE] = {};
//	char sendBuffer[BUFSIZE] = {};
//	int32 recvBytes = 0;
//};
//
//enum IO_TYPE
//{
//	READ,
//	WRITE,
//	ACCEPT,
//	CONNECT,
//};
//
//struct OverlappedEx
//{
//	WSAOVERLAPPED overlapped = {};
//	int32 type = 0; // read, write, accept, connect ...
//};
//
//void WorkerThreadMain(HANDLE iocpHandle)
//{
//	while (true)
//	{
//		DWORD bytesTransferred = 0;
//		Session* session = nullptr;
//		OverlappedEx* overlappedEx = nullptr;
//
//		BOOL ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred,
//			(ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);
//
//		if (ret == FALSE || bytesTransferred == 0)
//		{
//			// TODO : 연결 끊김
//			continue;
//		}
//
//		//ASSERT_CRASH(overlappedEx->type == IO_TYPE::READ);
//
//		cout << "Recv Data IOCP = " << bytesTransferred << " / " << session->recvBuffer << endl;
//
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUFSIZE;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//		::WSARecv(session->socket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//		::WSASend(session->socket, &wsaBuf, 1, &recvLen, flags, &overlappedEx->overlapped, NULL);
//
//		cout << "Send Data IOCP = " << bytesTransferred << " / " << session->recvBuffer << endl;
//	}
//}
//
//int main2()
//{
//	WSAData wsaData;
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		return 0;
//
//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (listenSocket == INVALID_SOCKET)
//		return 0;
//
//	SOCKADDR_IN serverAddr;
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//	serverAddr.sin_port = ::htons(7777);
//
//	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//		return 0;
//
//	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
//		return 0;
//
//	cout << "Accept" << endl;
//	
//	vector<Session*> sessionManager;
//
//	// CP 생성
//	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
//
//	// WorkerThreads
//	GThreadManager->Launch([=]() { WorkerThreadMain(iocpHandle); });
//
//	// Main Thread = Accept 담당
//	while (true)
//	{
//		SOCKADDR_IN clientAddr;
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
//		if (clientSocket == INVALID_SOCKET)
//			return 0;
//
//		Session* session = xnew<Session>();
//		session->socket = clientSocket;
//		sessionManager.push_back(session);
//
//		cout << "Client Connected ! Number = " << Session_Count++ << endl;
//
//		// 소켓을 CP에 등록
//		::CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, /*Key*/(ULONG_PTR)session, 0);
//
//		//Recv Buffer
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUFSIZE;
//
//		OverlappedEx* overlappedEx = new OverlappedEx();
//		overlappedEx->type = IO_TYPE::READ;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//
//		::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//	}
//
//	GThreadManager->Join();
//
//	// 윈속 종료
//	::WSACleanup();
//}