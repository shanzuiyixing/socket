#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include "host.h"
#pragma comment(lib,"Ws2_32.lib ")
#define DEFAULT_PORT "27015"
#define DEFAULT_SERVER "127.0.0.1"
#define DEBUG 1
Host::Host() {
    // The sockaddr_in structure specifies the address family,
   // IP address, and port for the socket that is being bound.
    listen_sock = INVALID_SOCKET;
    host_client_socket = INVALID_SOCKET;
    std::cout << "Host : addr family port" << std::endl;
        this->host_saServer.sin_family = AF_INET;
        this->host_saServer.sin_addr.S_un.S_addr = INADDR_ANY;
        this->host_saServer.sin_port = htons(27015);
#if  DEBUG
    std::cout << this->host_saServer.sin_addr.S_un.S_addr << std::endl;
    std::cout << this->host_saServer.sin_family << std::endl;
    std::cout << this->host_saServer.sin_port << std::endl;
    std::cout << "host is start" << std::endl;
#endif
    WSADATA wsaData;
    // 初始化 Winsock 库
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return;
    }
}

Host::~Host() {

#if  DEBUG
    std::cout << "host is over" << std::endl;
#endif

}
int Host::creat_host(){
    // Create a SOCKET for listening for 
    this->listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (this->listen_sock == INVALID_SOCKET) {
        wprintf(L"socket function failed with error: %u\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
#if  DEBUG
    std::cout <<"host socket is "<< this->listen_sock << std::endl;
#endif

    return 1;
}

int Host::bind_host() {
    int iResult = //bind(this->listen_sock, 
        //(SOCKADDR*)&this->host_saServer, sizeof(this->host_saServer));
    bind(this->listen_sock, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"bind failed with error %u\n", WSAGetLastError());
        closesocket(this->listen_sock);
        WSACleanup();
        return 1;
    }
    else
        wprintf(L"bind returned success\n");
    return 0;
}

int Host::listen_host() {
    int iResult = listen(this->listen_sock, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(this->listen_sock);
        WSACleanup();
        return 1;
    }
    std::cout << "Server is listening on port27015..." << std::endl;
    return 0;
}

int Host::accept_host(struct sockaddr_in client_saServer) {
    int clientAddrLen = sizeof(client_saServer);
    this->host_client_socket = accept(this->listen_sock, (sockaddr*)&client_saServer,
        &clientAddrLen);
    if (this->host_client_socket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(this->listen_sock);
        WSACleanup();
        return 1;
    }
    // No longer need server socket
    std::cout << "accept : Client connected." << std::endl;
    //closesocket(this->listen_sock);
    return 0;
}

int Host::recv_host() {
    int iResult = 1;
    int iSendResult = 1;
    // Receive until the peer shuts down the connection
    do {
        iResult = recv(this->host_client_socket, this->recvbuf, this->recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender
            iSendResult = send(this->host_client_socket, this->recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(this->host_client_socket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
            std::cout << this->recvbuf << std::endl;
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(this->host_client_socket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);
}

int Host::getaddr() {
    ZeroMemory(&this->hints, sizeof(this->hints));
    this->hints.ai_family = AF_INET;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_protocol = IPPROTO_TCP;
    this->hints.ai_flags =  AI_PASSIVE;
#if  DEBUG
    std::cout << "addr : " << std::endl;
    std::cout << this->hints.ai_family << std::endl;
    std::cout << this->hints.ai_socktype << std::endl;
    std::cout << this->hints.ai_protocol << std::endl;
    std::cout << this->hints.ai_flags << std::endl;
#endif
    // Resolve the server address and port
    int iResult = getaddrinfo(NULL, DEFAULT_PORT, &this->hints, &this->result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    return 1;
}

int Host::send_host() {
    iResult = send(this->listen_sock, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(this->listen_sock);
        WSACleanup();
        return 1;
    }

    printf("to client Bytes Sent: %ld\n", iResult);
}