#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include "stringUtils.h"
#include "ftpUtils.h"
#include "networkingUtils.h"
#include "handleUsername.h"
#include "handlePassword.h"
#include "handleStore.h"
#include "handleRetrieve.h"
#include "handleLogout.h"
#include "handleLst.h"
#include "handlePassive.h"
#include "ConnectedUser.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

char* passiveUsername = new char[BUFFER_SIZE];
char* passivePassword = new char[BUFFER_SIZE];


void handleRequest(ConnectedUser& user, SOCKET client, char* request) {
    const char* delimiters = " ";
    char* nextToken = NULL;
    std::cout << "Request: " << request << std::endl;
    char* cmd = strtok_s(request, delimiters, &nextToken);
    char* param = strtok_s(nullptr, delimiters, &nextToken);
    std::cout << "Command: " << cmd << std::endl;
    if (param != nullptr) 
        std::cout << "Parameter: " << param << std::endl;

    if (safeCmp(cmd, USER))
        handleUsername(client, user, param);
    else if (safeCmp(cmd, PASSWORD))
        handlePassword(client, user, param);
    else if (safeCmp(cmd, STORE))
        handleUploadFile(client, user, param);
    else if (safeCmp(cmd, RETR))
        handleDownloadFile(client, user, param);
    else if (safeCmp(cmd, LIST))
        handleListFiles(client, user, param);
    else if (safeCmp(cmd, PASSIVE)) {

        strcpy_s(passiveUsername, sizeof(passiveUsername), user.getUsername());
        strcpy_s(passivePassword, sizeof(passivePassword), user.getPassword());
        thread t(handlePassive, client,passiveUsername, passivePassword, user.getStatus());
        t.detach();
        //handlePassive(client, user);
    }
    else if (safeCmp(cmd, QUIT)) 
        handleLogout(client, user);
    else
        send(client, COMMAND_NOT_IMPLEMENTED, (int) strlen(COMMAND_NOT_IMPLEMENTED), 0);
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return EXIT_FAILURE;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Set up the server address struct
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind the socket to a specific address and port
    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (1) {
        // Accept incoming connections
        sockaddr_in clientAddress;
        int clientAddressLength = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddressLength);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return EXIT_FAILURE;
        }

        std::cout << "new client accepted\n";
        send(clientSocket, CONNECTION_SUCCESS, strlen(CONNECTION_SUCCESS), 0);
        ConnectedUser user;

        char buffer[BUFFER_SIZE];
        int bytesRead;

        while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
            if (!checkNullTerminated(buffer))
                buffer[bytesRead] = '\0';

            if (checkInput(buffer))
                handleRequest(user, clientSocket, buffer);
            else send(clientSocket, BAD_SYNTAX, (int)strlen(BAD_SYNTAX), 0);

            for (int i = 0; i < bytesRead; i++)
                buffer[i] = '\0';
        }
    
        std::cout << "Client disconnected" << std::endl;
        user.setStatus(0);
        closesocket(clientSocket);
    }

    delete passiveUsername;
    delete passivePassword;

    closesocket(serverSocket);
    WSACleanup();

    return EXIT_SUCCESS;

}
