#ifndef CONNECTEDUSER
#define CONNECTEDUSER

#include "stringUtils.h"

class ConnectedUser {
private:
    char* username;
    char* password;
    int loginStatus;
    

public:

    ConnectedUser() {
        username = new char[BUFFER_SIZE];
        password = new char[BUFFER_SIZE];
        loginStatus = 0;
    }

    void setUsername(char* newUsername) {
        strcpy_s(username, sizeof(username), newUsername);
    }

    void setPassword(char* newPassword) {
        strcpy_s(password, sizeof(password), newPassword);
    }
    void setStatus(int status) {
        loginStatus = status;
    }

    char* getUsername() const {
        return username;
    }

    char* getPassword() const {
        return password;
    }

    int getStatus() const {
        return loginStatus;
    }

    ~ConnectedUser() {
        delete username;
        delete password;
    }
};
#endif
