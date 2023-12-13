#ifndef HANDLEPASSIVE_H
#define HANDLEPASSIVE_H

#include "networkingUtils.h"
#include "ftpUtils.h"
#include "stringUtils.h"
#include "handleRetrieve.h"
#include "handleStore.h"
#include "ConnectedUser.h"
#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;
using namespace std;

void handlePassiveRequest(SOCKET client, ConnectedUser& user, char* request) {

    cout << "passive request\n";
    cout << user.getStatus() << "\n";
    cout << user.getUsername() << "\n";
    cout << user.getPassword() << "\n";
    cout << "\n";
    const char* delimiters = " ";
    char* nextToken = NULL;
    std::cout << "Passive Request: " << request << std::endl;
    char* cmd = strtok_s(request, delimiters, &nextToken);
    char* param = strtok_s(nullptr, delimiters, &nextToken);
    std::cout << "Passive Command: " << cmd << std::endl;
    if (param != nullptr)
        std::cout << "Parameter: " << param << std::endl;

    if (safeCmp(cmd, STORE))
        handleUploadFile(client, user, param);
    else if (safeCmp(cmd, RETR))
        handleDownloadFile(client, user, param);
    else
        send(client, COMMAND_NOT_IMPLEMENTED, (int)strlen(COMMAND_NOT_IMPLEMENTED), 0);
}

void handlePassive(SOCKET client, char* username, char* password, int loginStatus) {
    ConnectedUser user;
    user.setUsername(username);
    user.setPassword(password);
    user.setStatus(loginStatus);

    if (user.getStatus() != 2) {
        send(client, LOGIN_FAILED, (int)strlen(LOGIN_FAILED), 0);
        return;
    }


    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        send(client, SERVICE_NOT_AVAILABLE, strlen(SERVICE_NOT_AVAILABLE), 0);
        return;
    }

    // Set up the server address struct
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PASSIVE_PORT);

    // Bind the socket to a specific address and port
    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        send(client, SERVICE_NOT_AVAILABLE, strlen(SERVICE_NOT_AVAILABLE), 0);
        closesocket(serverSocket);
        return;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        closesocket(serverSocket);
        send(client, SERVICE_NOT_AVAILABLE, strlen(SERVICE_NOT_AVAILABLE), 0);
        return;
    }

    std::cout << "Server listening passive on port " << PASSIVE_PORT << std::endl;

    char passivePort[BUFFER_SIZE];
    strcpy_s(passivePort, sizeof(passivePort), to_string(PASSIVE_PORT).c_str());
    send(client, passivePort, (int)strlen(passivePort), 0);
    //send(client, PASSIVE_MODE, strlen(PASSIVE_MODE), 0);


    // Accept incoming connections
    sockaddr_in clientAddress;
    int clientAddressLength = sizeof(clientAddress);
    SOCKET clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddressLength);
    if (clientSocket == INVALID_SOCKET) {
        send(client, SERVICE_NOT_AVAILABLE, strlen(SERVICE_NOT_AVAILABLE), 0);
        closesocket(clientSocket);
        closesocket(serverSocket);
        return;
    }

    cout << "accepted client on passive connection\n";


    char buffer[BUFFER_SIZE];
    int bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        if (!checkNullTerminated(buffer))
            buffer[bytesRead] = '\0';

        if (checkInput(buffer))
            handlePassiveRequest(clientSocket, user, buffer);
        else send(clientSocket, BAD_SYNTAX, (int)strlen(BAD_SYNTAX), 0);

        for (int i = 0; i < bytesRead; i++)
            buffer[i] = '\0';
    }

    cout << bytesRead << "\n";
}
#endif
