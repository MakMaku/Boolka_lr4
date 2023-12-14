#include "TClient.h"

void TClient::Creat_Socket() {
    // initialise winsock
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        printf("Failed. Error Code: %d", WSAGetLastError());
    }
    printf("Initialised.\n");

    // create socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
    {
        printf("socket() failed with error code: %d", WSAGetLastError());
    }

    // setup address structure
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    char* addres = new char[SERVER.length() + 1];
    strcpy(addres, SERVER.c_str());
    server.sin_addr.S_un.S_addr = inet_addr(addres);
}

void TClient::Send(char* mes) {
    // send the message
    if (sendto(client_socket, mes, sizeof(mes), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR){
        printf("sendto() failed with error code: %d", WSAGetLastError());
    }
}

void TClient::Clean_n_Close() {
    closesocket(client_socket);
    WSACleanup();
}