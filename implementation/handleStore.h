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

    if (!checkFileFormat(filepath))
        send(client, WRONG_FORMAT, (int)strlen(WRONG_FORMAT), 0);

    fs::path currentPath = fs::current_path().parent_path().parent_path();
    currentPath = currentPath / "implementation" / user.getUsername() / filepath;
    //currentPath = currentPath / filepath;
    fs::create_directories(currentPath.parent_path());
    std::ofstream fout(string(SERVER_PATH) + "\\" + user.getUsername() + "\\" + string(filepath), ios::out | ios::binary);

    cout << currentPath << "\n";

    if (fout.is_open()) {
        char buffer[BUFFER_SIZE];
        int bytesRead;
        while ((bytesRead = recv(client, buffer, sizeof(buffer), 0)) > 0) {
            buffer[bytesRead] = '\0';
            //cout << "received " << bytesRead << "\n";
            fout.write(buffer, bytesRead);
            if (fout.fail()) {
                cout << "failed writing\n";
                send(client, LOCAL_ERROR, (int)strlen(LOCAL_ERROR), 0);
                return;
            }
        }
        fout.close();
    }
    else
        send(client, LOCAL_ERROR, (int)strlen(LOCAL_ERROR), 0);
}
#endif