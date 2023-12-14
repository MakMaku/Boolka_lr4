#pragma once
#include <iostream>
#include <winsock2.h>
#include "Struct.h"
#pragma comment(lib,"ws2_32.lib") // Winsock Library
#pragma warning(disable:4996) 

class TServer{
private:
	sockaddr_in server, client;
	int PORT = 0000;
	WSADATA wsa;
	SOCKET server_socket;
public:
	void set_port(int port) {
		this->PORT = port;
	}
	void Creat_Socket();
	BNR Receive();
	void Clean_n_Close();
};

