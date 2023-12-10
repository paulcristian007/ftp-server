#ifndef HANDLEUSERNAME_H
#define HANDLEUSERNAME_H

#include "networkingUtils.h"
#include "ftpUtils.h"
#include "stringUtils.h"
#include <fstream>
#include <string>
#include "ConnectedUser.h"
using namespace std;

void handleUsername(SOCKET client,ConnectedUser& user, char username[]) {
    user.setUsername(username);
    if (user.getStatus() == 2) {
        send(client, LOGIN_SUCCESS, (int)strlen(LOGIN_SUCCESS), 0);
        return;
    }
    if (user.getStatus() == 1) {
        send(client, USERNAME_OK, (int)strlen(USERNAME_OK), 0);
    }


    ifstream fin(string(SERVER_PATH) + string("\\accounts.txt"));
    if (!fin || !fin.is_open()) {
        send(client, SERVICE_NOT_AVAILABLE, (int)strlen(SERVICE_NOT_AVAILABLE), 0);
        return;
    }

    char line[BUFFER_SIZE];
    while (fin.getline(line, sizeof(line)))
        if (checkCredentials(line)) {
            const char* delimiters = ";";
            char* nextToken = NULL;
            char* usernm= strtok_s(line, delimiters, &nextToken);
            char* password = strtok_s(nullptr, delimiters, &nextToken);

            if (safeCmp(usernm, username)) {
                send(client, USERNAME_OK, (int)strlen(USERNAME_OK), 0);
                user.setStatus(1);
                user.setPassword(password);
                return;
            }
        }

    send(client, NO_ACCOUNT, (int)strlen(NO_ACCOUNT), 0);
}
#endif