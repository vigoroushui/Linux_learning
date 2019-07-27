#include "ftp.h"

void identityConfirm(int socketFd)
{
    char *userName,*password;                                                       
    userName=getpass("please input usrname:");
    password=getpass("please input passwd:");
    int nameLen=strlen(userName);
    send(socketFd,&nameLen,sizeof(int),0);
    sendCycle(socketFd,userName,nameLen);
    int pwdLen=strlen(password);
    send(socketFd,&pwdLen,sizeof(int),0);
    sendCycle(socketFd,password,pwdLen);
}
