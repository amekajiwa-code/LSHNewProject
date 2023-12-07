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
	// IPv4, TCP, 기본 프로토콜
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	//오류 예외처리
	if (clientSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket Error : " << errCode << endl;
		return 0;
	}

	//목적지 설정
	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr)); // 구조체 비우기
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);
	// htons : host to network short

	//패킷 보내기
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Connect Error : " << errCode << endl;
		return 0;
	}

	cout << "Connected To Server!" << endl;
	//TODO
	while (true)
	{
		char sendBuffer[100] = "Hello World";
		int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		if (resultCode == SOCKET_ERROR)
		{
			int32 errCode = ::WSAGetLastError();
			cout << "Send Error : " << errCode << endl;
			return 0;
		}

		cout << "Send Data Len = " << sizeof(sendBuffer) << endl;

		this_thread::sleep_for(1s);
	}

	//소켓 리소스 반환
	::closesocket(clientSocket);
	//윈속 종료
	::WSACleanup();
}