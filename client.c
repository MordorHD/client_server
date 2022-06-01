#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdbool.h>

#define IP_ADDRESS "" // your address
#define PORT 0 // some port

SOCKET user_connect(void)
{
    SOCKET sock = INVALID_SOCKET;
    int iResult;

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family       = AF_INET;
    addr.sin_addr.s_addr  = inet_addr(IP_ADDRESS);
    addr.sin_port         = htons(PORT);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    iResult = connect(sock, (struct sockaddr*) &addr, sizeof(addr));
    if(iResult == SOCKET_ERROR)
    {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

int main(int argc, char **argv)
{
    SOCKET sock;
    WSADATA wsaData;
    INT iRetval;
    printf("> Connecting to server...\n");
    if((iRetval = WSAStartup(MAKEWORD(2, 2), &wsaData)))
    {
        printf("> error: WSAStartup failed (error code: 0xF0%x)\n", iRetval);
        return 0xFF;
    }
    sock = user_connect();
    if(sock == INVALID_SOCKET)
        printf("> error: couldn't connect to server\n");
    else // everything went fine
        closesocket(sock);
    WSACleanup();
    return 0;
}
