#pragma once
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
class Host {
public:
	SOCKET listen_sock;
	SOCKET host_client_socket;
	struct sockaddr_in host_saServer;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	int iResult;
	const char* sendbuf = "this is a test from host to client";
	Host();
	~Host();
	int creat_host();
	int bind_host();
	int listen_host();
	int accept_host(struct sockaddr_in client_saServer);
	int send_host();
	int recv_host();
	int close_host();
	int getaddr();
};