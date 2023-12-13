#ifndef HANDLESTORE_H
#define HANDLESTORE_H

#include "networkingUtils.h"
#include "ftpUtils.h"
#include "stringUtils.h"
#include "ConnectedUser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;
using namespace std;


void handleUploadFile(SOCKET client, ConnectedUser& user, char filepath[]) {
    cout << "try to create\n";
    if (user.getStatus() != 2) {
        send(client, LOGIN_FAILED, (int)strlen(LOGIN_FAILED), 0);
        return;
    }

    if (!checkFileFormat(filepath)) {
        send(client, WRONG_FORMAT, (int)strlen(WRONG_FORMAT), 0);
        return;
    }

    fs::path currentPath = fs::current_path().parent_path();
    currentPath = currentPath / "implementation" / user.getUsername() / filepath;
    //currentPath = currentPath / filepath;
    fs::create_directories(currentPath.parent_path());
    std::ofstream fout(currentPath, ios::out | ios::binary); // Open the file using currentPath

    cout << currentPath << "\n";

    if (fout.is_open()) {
        char buffer[BUFFER_SIZE];
        int totalBytesReceived = 0;
        int bytesReceived;

        // Receive file size as the first message
        std::streampos fileSize;
        recv(client, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

        while ((bytesReceived = recv(client, buffer, sizeof(buffer), 0)) > 0) {
            totalBytesReceived += bytesReceived;
            if (totalBytesReceived >= fileSize) {
                fout.write(buffer, static_cast<std::streamsize>(fileSize) - totalBytesReceived + bytesReceived);
                break;
            }
            else {
                fout.write(buffer, bytesReceived);
            }
        }

        if (totalBytesReceived != fileSize) {
            cout << "Incomplete file received\n";
            send(client, LOCAL_ERROR, (int)strlen(LOCAL_ERROR), 0);
            fout.close();
            return;
        }

        fout.close();
        cout << "File: " << filepath << " stored successfully" << endl;
    }
    else {
        send(client, LOCAL_ERROR, (int)strlen(LOCAL_ERROR), 0);
    }
}
#endif
