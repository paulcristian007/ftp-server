#ifndef HANDLEPASSWORD_H
#define HANDLEPASSWORD_H

#include "networkingUtils.h"
#include "ftpUtils.h"
#include "stringUtils.h"
#include "ConnectedUser.h"


void handlePassword(SOCKET client, ConnectedUser& user, char password[]) {
    if (user.getStatus() == 0) {
        send(client, BAD_COMMAND_SEQUENCE, (int)strlen(LOGIN_SUCCESS), 0);
        return;
    }
    if (user.getStatus() == 2 || safeCmp(password, user.getPassword())) {
        send(client, LOGIN_SUCCESS, (int)strlen(LOGIN_SUCCESS), 0);
        user.setStatus(2);
        return;
    }
    send(client, LOGIN_FAILED, (int)strlen(LOGIN_FAILED), 0);
}

#endif