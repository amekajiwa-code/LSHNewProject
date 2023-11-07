#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>

const short port = 8080;


int main()
{
	WSADATA wsa; // 윈소켓 구조체
	int ret = 0;

	ret = WSAStartup(MAKEWORD(2, 2), &wsa); // 소켓 초기화
	if (ret != 0) return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	//논블록형 소켓 : 작업이 완료되기 전에 반환
	u_long on = TRUE;
	ioctlsocket(sock, FIONBIO, &on);

	SOCKADDR_IN socketAdd; // 소켓 주소 구조체
	socketAdd.sin_family = AF_INET; // 필드 주소체계 설정 AF_INET는 IPv4
	socketAdd.sin_addr.s_addr = htonl(INADDR_ANY); // 내부 IP주소 설정
	// INADDR_ANY는 네트워크 인터페이스로 어떤 주소든 바인딩되어 들어올 수 있도록 함
	socketAdd.sin_port = htons(port); // 포트번호 설정

	ret = bind(sock, (SOCKADDR*)&socketAdd, sizeof(socketAdd));
	if (ret == SOCKET_ERROR) return 1;

	ret = listen(sock, SOMAXCONN);
	if (ret == SOCKET_ERROR) return 1;

	SOCKADDR_IN clientAdd;
	SOCKET clientSoc;

	printf("서버 시작\n");

	while (1)
	{
		int addLen = sizeof(clientAdd);
		clientSoc = accept(sock, (SOCKADDR*)&clientAdd, &addLen);

		if (clientSoc == SOCKET_ERROR)
		{
			int wsaError = WSAGetLastError();
			if (wsaError != WSAEWOULDBLOCK) break;
		}
		else
		{
			printf("클라이턴트 접속 ip = %s , Port : %d\n",
				inet_ntoa(clientAdd.sin_addr),
				ntohs(clientAdd.sin_port));

			while (1)
			{
				char buffer[256] = { 0, };
				int recvByte = recv(clientSoc, buffer, 256, 0);

				if (recvByte == SOCKET_ERROR)
				{
					int wsaError = WSAGetLastError();
					if (wsaError != WSAEWOULDBLOCK) break;
				}
				else
				{
					if (recvByte == 0) break; // 정상 종료
					if (recvByte == SOCKET_ERROR) break; //비정상 종료

					printf("[받음] %s\n", buffer);

					int bufferLen = strlen(buffer);
					int sendByte = send(clientSoc, buffer, bufferLen, 0);

					if (sendByte == SOCKET_ERROR)
					{
						int wsaError = WSAGetLastError();
						if (wsaError != WSAEWOULDBLOCK) break;
					}

					printf("%d바이트를 전송했습니다.", sendByte);
				}
			}

			closesocket(clientSoc);
			printf("클라이언트 접속해제 ip = %s, Port : %d\n",
				inet_ntoa(clientAdd.sin_addr),
				ntohs(clientAdd.sin_port));
		}
	}

	closesocket(sock);
	// winsoc 해제
	WSACleanup();
	printf("완전 해제");
}