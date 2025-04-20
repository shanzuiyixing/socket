#pragma once
#include "client.h"
#include "host.h"
#define DEBUG 0


class main {
public:
	void thread_host(Host host, Client client);
	void thread_client(Host host, Client client);
};
