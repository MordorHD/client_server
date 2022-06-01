#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdbool.h>

#define IP_ADDRESS "127.0.0.1" // your IP address
#define PORT 0 // your port

SOCKET start_server(void)
{
    INT iResult;
    SOCKET sock;

    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        printf("> error: socket failed (error code: %d)\n", WSAGetLastError());
        return INVALID_SOCKET;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family       = AF_INET;
    addr.sin_addr.s_addr  = inet_addr(IP_ADDRESS);
    addr.sin_port         = htons(PORT);

    iResult = bind(sock, (struct sockaddr*) &addr, sizeof(addr));
    if(iResult == SOCKET_ERROR)
    {
        printf("> error: bind failed (error code: %d)\n", WSAGetLastError());
        closesocket(sock);
        return INVALID_SOCKET;
    }
    return sock;
}

int main(int argc, char **argv)
{
    char buf[1024];
    SOCKET serverSock;
    SOCKET socks[2];
    WSADATA wsaData;
    INT iRetval;
    if((iRetval = WSAStartup(MAKEWORD(2, 2), &wsaData)))
    {
        printf("> error: Startup failed(error code: 0xF0%x)\n", iRetval);
        return 0xFF;
    }
    printf("> Connecting to server...\n");
    serverSock = start_server();
    if(serverSock == INVALID_SOCKET)
    {
        printf("> error: invalid server socket\n");
        WSACleanup();
        return 0xFF;
    }
    printf("> Connected to server...\n");
    printf("> Listening for connections..\n");
    // wait for two clients two join
    for(int i = 0; i < sizeof(socks) / sizeof(*socks); )
    {
        iRetval = listen(serverSock, 0);
        if(iRetval == SOCKET_ERROR)
        {
            printf("> error: listen\n");
            continue;
        }
        socks[i] = accept(serverSock, NULL, NULL);
        if(socks[i] == INVALID_SOCKET)
            continue;
        printf("> Client %d connected\n", i);
        i++;
    }
    return 0;
}
