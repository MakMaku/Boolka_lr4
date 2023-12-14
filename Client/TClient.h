#pragma once
#include <iostream>
#include <vector>
#include <winsock2.h>
#include "Struct.h" 
#include <chrono>

using namespace std;
using namespace chrono;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

class TClient{
protected:
	string SERVER = "localhost";	//ip address of UDP server
	int PORT = 23446;				//the port on which to listen for incoming data
	WSADATA ws;
	sockaddr_in server;
	int client_socket;
public:
	void set_port(int port) {
		this->PORT = port;
	}
	void set_ipaddres(string ip) {
		this->SERVER = ip;
	}
	void Creat_Socket();
	void Send(char* mes);
	void Clean_n_Close();
	virtual void Working() = 0;
	virtual void Sending() = 0;
};

