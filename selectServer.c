#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Move to header 
#define PORT    32001

// I'm going to move this into another file and keep non-main logic there. 
void handle(SOCKET newsock, fd_set *set)
{
}
 
int main(void)
{
#ifndef LINUX
    WORD wVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
#endif /* Windows */
    int iResult;
    SOCKET sock;
    fd_set socks;
    fd_set readsocks;
    SOCKET maxsock;
    int reuseaddr = 1; /* True */
    struct addrinfo hints, *res;
 
    /* Initialise Winsock */
    if (iResult = (WSAStartup(wVersion, &wsaData)) != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
 
    /* Get the address info */
    ZeroMemory(&hints, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        WSACleanup();
        return 1;
    }
 
    /* Create the socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        WSACleanup();
        return 1;
    }
 
    /* Enable the socket to reuse the address */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseaddr, 
                sizeof(int)) == SOCKET_ERROR) {
        perror("setsockopt");
        WSACleanup();
        return 1;
    }
 
    /* Bind to the address */
    if (bind(sock, res->ai_addr, res->ai_addrlen) == SOCKET_ERROR) {
        perror("bind");
        WSACleanup();
        return 1;
    }
 
    /* Listen */
    if (listen(sock, BACKLOG) == SOCKET_ERROR) {
        perror("listen");
        WSACleanup();
        return 1;
    }
 
    /* Set up the fd_set */
    FD_ZERO(&socks);
    FD_SET(sock, &socks);
    maxsock = sock;
 
    /* Main loop */
    while (1) {
        SOCKET s;
        readsocks = socks;
        if (select(maxsock + 1, &readsocks, NULL, NULL, NULL) == SOCKET_ERROR) {
            perror("select");
            WSACleanup();
            return 1;
        }
        for (s = 0; s <= maxsock; s++) {
            if (FD_ISSET(s, &readsocks)) {
                printf("Socket %d was ready\n", s);
                if (s == sock) {
                    /* New connection */
                    SOCKET newsock;
                    struct sockaddr_in their_addr;
                    size_t size = sizeof(struct sockaddr_in);
 
                    newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
                    if (newsock == INVALID_SOCKET) {
                        perror("accept");
                    }
                    else {
                        printf("Got a connection from %s on port %d\n", 
                                inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
                        FD_SET(newsock, &socks);
                        if (newsock > maxsock) {
                            maxsock = newsock;
                        }
                    }
                }
                else {
                    /* Handle read or disconnection */
                    handle(s, &socks);
                }
            }
        }
    }
 
    /* Clean up */
    closesocket(sock);
    WSACleanup();
 
    return 0;
}
