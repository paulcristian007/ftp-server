#ifndef HANDLELST_H
#define HANDLELST_H

#include "networkingUtils.h"
#include "ftpUtils.h"
#include "stringUtils.h"
#include "ConnectedUser.h"
#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;
using namespace std;

void handleListFiles(SOCKET client, ConnectedUser& user, char* path) {
    if (user.getStatus() != 2) {
        send(client, LOGIN_FAILED, (int)strlen(LOGIN_FAILED), 0);
        return;
    }

    fs::path currentPath = fs::current_path().parent_path().parent_path();
    currentPath = currentPath / "implementation" / user.getUsername();
    if (path != nullptr)
        currentPath = currentPath / path;

    if (!fs::exists(currentPath) || !fs::is_directory(currentPath)) {
        send(client, WRONG_FORMAT, (int)strlen(WRONG_FORMAT), 0);
        return;
    }
    for (const auto& entry : fs::directory_iterator(currentPath))
        if (fs::is_regular_file(entry)) {
            std::cout << entry.path().filename().string() << std::endl;
            char filename[BUFFER_SIZE];
            strcpy_s(filename, sizeof(filename), entry.path().filename().string().c_str());
            send(client, filename, (int)strlen(filename), 0);
        }
}

#endif