#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "client.h"
#pragma comment(lib, "ws2_32.lib") // 链接 Winsock 库
#define DEBUG 1
#define DEFAULT_PORT "27015"
#define DEFAULT_SERVER "127.0.0.1"

Client::Client() {
    std::cout << "client family addr port" << std::endl;
    this->client_saServer.sin_family = AF_INET;
    this->client_saServer.sin_addr.S_un.S_addr = INADDR_ANY;
    this->client_saServer.sin_port = htons(8080);
#if  DEBUG
    std::cout << this->client_saServer.sin_port << std::endl;
    std::cout << this->client_saServer.sin_family << std::endl;
    std::cout << this->client_saServer.sin_addr.S_un.S_addr << std::endl;
#endif
    iResult = getaddrinfo(DEFAULT_SERVER, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
    }
    std::cout << "client is right" << std::endl;
    int iResult = 0;
    argc = 2;
}
Client::~Client() {
#if  DEBUG
    std::cout << "client is done" << std::endl;
#endif
}
int Client::creat_client() {
    WSADATA wsaData;
    // 初始化 Winsock 库
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }
    this->client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (this->client_sock == INVALID_SOCKET) {
        wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
#if  DEBUG
    std::cout << this->client_sock << std::endl;
#endif
}
int Client::connect_client(struct sockaddr_in host_saServer) {
    iResult = getaddrinfo(DEFAULT_SERVER, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
     //Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // Create a SOCKET for connecting to server
        if (this->client_sock == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        // Connect to server.
        iResult = connect(this->client_sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(this->client_sock);
            this->client_sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    if (this->client_sock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send(this->client_sock, sendbuf, (int)strlen(sendbuf), 0);

    if (this->client_sock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
    printf("Bytes Sent: %ld\n", iResult);
    //// shutdown the connection since no more data will be sent
    //iResult = shutdown(this->client_sock, SD_SEND);
    //if (iResult == SOCKET_ERROR) {
    //    printf("shutdown failed with error: %d\n", WSAGetLastError());
    //    closesocket(this->client_sock);
    //    WSACleanup();
    //    return 1;
    //}

}

int Client::read_client() {
    int iResult = 0;
    // Receive until the peer closes the connection
    do {    

        iResult = recv(this->client_sock, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);
    return 0;
}

int Client::write_client() {
    // Send an initial buffer
    int iResult = send(this->client_sock, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(this->client_sock);
        WSACleanup();
        return 1;
    }

    printf("write : Bytes Sent: %ld\n", iResult);
    return 0;
}

int Client::close_client() {
    int iResult = closesocket(this->client_sock);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
#if  DEBUG
    std::cout << this->client_sock << "is over" << std::endl;
#endif
}