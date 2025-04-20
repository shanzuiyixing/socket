#pragma once
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
class Client {
public:
	int argc;
	char** argv;
	SOCKET client_sock;
	struct sockaddr_in client_saServer;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	const char* sendbuf = "oh my god weishidaier master";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	Client();
	~Client();
	int creat_client();
	int connect_client(struct sockaddr_in host_saServer);
	int write_client();
	int read_client();
	int close_client();
};