#include "TServer.h"
void TServer::Creat_Socket() {
    // initialise winsock
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code: %d", WSAGetLastError());
        exit(0);
    }
    printf("Initialised.\n");

    // create a socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket: %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    // prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // bind
    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done.");
}

BNR TServer::Receive() {
    BNR Message;
    //printf("Waiting for data...");
    fflush(stdout);
    // try to receive some data, this is a blocking call
    int message_len;
    int slen = sizeof(sockaddr_in);
    if (message_len = recvfrom(server_socket, (char*)&Message, sizeof(Message), 0, (sockaddr*)&client, &slen) == SOCKET_ERROR)
    {
        //printf("recvfrom() failed with error code: %d", WSAGetLastError());
        //exit(0);
    }
    return Message;
}

void TServer::Clean_n_Close() {
    closesocket(server_socket);
    WSACleanup();
}