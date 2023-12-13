#ifndef HANDLERETRIEVE_H
#define HANDLERETRIEVE_H

#include "networkingUtils.h"
#include "ftpUtils.h"
#include "stringUtils.h"
#include "ConnectedUser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;
using namespace std; 

void handleDownloadFile(SOCKET client, ConnectedUser& user, char filepath[]) {

    cout << "download\n";
    cout << user.getStatus() << "\n";
    cout << user.getUsername() << "\n";
    cout << user.getPassword() << "\n";
    cout << "\n";
    if (user.getStatus() != 2) {
        send(client, LOGIN_FAILED, (int)strlen(LOGIN_FAILED), 0);
        return;
    }

    if (!checkFileFormat(filepath))
        send(client, WRONG_FORMAT, (int)strlen(WRONG_FORMAT), 0);


    ifstream file(string(SERVER_PATH) + "\\" + user.getUsername() + "\\" + string(filepath), ios::in | ios::binary);

    if (file.is_open()) {
        cout << "start read\n";
        
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        char buffer[BUFFER_SIZE + 1];

        std::streamsize bytesRead = 0;
        send(client, reinterpret_cast<const char*>(&fileSize), sizeof(fileSize), 0);

        while (bytesRead < fileSize) {
            file.read(buffer, BUFFER_SIZE);
            int bytesToRead = static_cast<int>(file.gcount());
            int bytesSent = send(client, buffer, bytesToRead, 0);

            if (bytesSent == SOCKET_ERROR || bytesSent <= 0) {
                std::cerr << "Error sending file" << std::endl;
                break;
            }

            bytesRead += bytesSent;
        }
        file.close();
        cout << "File sent successfully";
    }
    else {
        cout << "cannot find " << filepath;
        send(client, LOCAL_ERROR, (int)strlen(LOCAL_ERROR), 0);
    }
}

#endif