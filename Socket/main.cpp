#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "main.h"
#include <thread>
#pragma comment(lib, "ws2_32.lib") // 链接 Winsock 库
#define DEBUG 1
void thread_host(Host host, Client client) {
	host.getaddr();
	host.creat_host();
	host.bind_host();
	host.listen_host();
	host.accept_host(client.client_saServer);
	host.recv_host();
	//host.send_host();
}
void thread_client(Host host, Client client) {
	client.creat_client();
	client.connect_client(host.host_saServer);
	client.write_client();
	//client.read_client();
	//client.close_client();
}

int main() {
	Client client_1;
	Host host_1;

	std::thread t_host(thread_host,host_1,client_1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::thread t_client(thread_client,host_1,client_1);
	t_host.join();
	t_client.join();

}