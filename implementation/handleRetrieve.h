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


    ifstream fin(string(SERVER_PATH) + "\\" + user.getUsername() + "\\" + string(filepath), ios::in | ios::binary);

    if (fin.is_open()) {
        cout << "start read\n";
        char buf[BUFFER_SIZE + 1];
        fin.read(buf, sizeof(buf));

        while (fin.gcount() > 0) {
            cout << "read " << fin.gcount() << "bytes\n";
            //int x = 
            send(client, buf, (int)fin.gcount(), 0);
            //cout << "sent " << x << "bytes\n\n";
            fin.read(buf, sizeof(buf) - 1);
        }
        fin.close();
        closesocket(client);
    }
    else {
        cout << "cannot find " << filepath;
        send(client, LOCAL_ERROR, (int)strlen(LOCAL_ERROR), 0);
    }
}

#endif