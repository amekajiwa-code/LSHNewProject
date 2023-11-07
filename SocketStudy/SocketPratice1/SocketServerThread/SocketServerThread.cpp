#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
const short port = 8080;

std::list<SOCKET>  g_userlist;

int main()
{
    // 1) winsock 초기화
    WSADATA wsa;
    int iRet = 0;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iRet != 0) return 1;
    //SOCKET sockTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //SOCKET sockUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // stream 일때 TCP dgram일때 udp

    // 넌블록형 소켓 할당
    u_long on = TRUE;
    ioctlsocket(sock, FIONBIO, &on);

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port);

    iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa)); // bind : 서버소켓을 특정 ip주소와 포트에 바인딩
    if (iRet == SOCKET_ERROR) return 1;
     
    iRet = listen(sock, SOMAXCONN); // 서버 소켓에서 클라이언트 연결을 받아들일수 있도록 대기 상태
    if (iRet == SOCKET_ERROR) return 1;

    SOCKADDR_IN clientaddr;
    SOCKET clientsock;

    std::clock_t start = std::clock();
    printf("서버 정상 작동 시작!!\n");
    while (1) {
        int addlen = sizeof(clientaddr);
        clientsock = accept(sock, (SOCKADDR*)&clientaddr, &addlen);
        if (clientsock == SOCKET_ERROR)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                break;
            }
        }
        else
        {
            g_userlist.push_back(clientsock);
            printf("클라이언트 접속 ip=%s, Port:%d\n",
                inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port));
        }
        for (std::list<SOCKET>::iterator iter = g_userlist.begin();
            iter != g_userlist.end();
            )
        {
            SOCKET cSock = *iter;
            char buf[256] = { 0, };
            int iRecvByte = recv(cSock, buf, 256, 0);
            if (iRecvByte == SOCKET_ERROR)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    iter = g_userlist.erase(iter);
                    continue;
                }
            }
            else
            {
                if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
                {
                    iter = g_userlist.erase(iter);
                    continue;
                }

                printf("[받음]%s\n", buf);
                int iLen = strlen(buf);

                for (std::list<SOCKET>::iterator iterSend = g_userlist.begin();
                    iterSend != g_userlist.end();
                    iterSend++)
                {
                    SOCKET cSock = *iterSend;

                    int iSendByte = send(cSock, buf, iLen, 0);
                    if (iSendByte == SOCKET_ERROR)
                    {
                        int iError = WSAGetLastError();
                        if (iError != WSAEWOULDBLOCK)
                        {
                            break;
                        }
                    }
                }

            }
            iter++;
             /*closesocket(clientsock);
             printf("클라이언트 접속해제 ip=%s, Port:%d\n",
                 inet_ntoa(clientaddr.sin_addr),
                 ntohs(clientaddr.sin_port));*/
        }
    }
    closesocket(sock);
    // )  winsock 해제    
    WSACleanup();
    std::cout << "Hello World!\n";
}