#ifndef HANDLELOGOUT_H
#define HANDLELOGOUT_H

#include "networkingUtils.h"
#include "ftpUtils.h"
#include "stringUtils.h"

void handleLogout(SOCKET client, ConnectedUser& user) {
    user.setStatus(0);
    send(client, CONNECTION_CLOSE, (int)strlen(CONNECTION_CLOSE), 0);
    closesocket(client);
}

#endif