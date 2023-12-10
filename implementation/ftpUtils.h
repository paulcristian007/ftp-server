#ifndef FTPUTILS_H
#define FTPUTILS_H

const char* USER = "USER";
const char* PASSWORD = "PASS";
const char* PASSIVE = "PASV";
const char* STORE = "STOR";
const char* RETR = "RETR";
const char* LIST = "NLST";
const char* QUIT = "QUIT";

const char* CONNECTION_SUCCESS = "220 - Service ready for new user.";
const char* CONNECTION_CLOSE = "221 - Service closing control connection. Logged out if appropriate";
const char* FILE_SENT = "226 - Closing data connection. Requested file action successful";
const char* PASSIVE_MODE = "227 - Entering Passive Mode";
const char* LOGIN_SUCCESS = "230 - User logged in, proceed.";
const char* USERNAME_OK = "331 - User name okay, need password.";
const char* NO_ACCOUNT = "332 - Need account for login.";
const char* SERVICE_NOT_AVAILABLE = "421 - Service not available, closing control connection.";
const char* LOCAL_ERROR = "451 Requested action aborted: local error in processing.";
const char* BAD_SYNTAX = "500 - Syntax error, command unrecognized.This may include errors such as command line too long.";
const char* WRONG_FORMAT = "501 - Syntax error in parameters or arguments.";
const char* COMMAND_NOT_IMPLEMENTED = "502 - Command not implemented.";
const char* BAD_COMMAND_SEQUENCE = "503 - Bad sequence of commands.";
const char* LOGIN_FAILED = "530 - Not logged in.";
const char* STORE_NO_LOGIN = "532 Need account for storing files.";
#endif