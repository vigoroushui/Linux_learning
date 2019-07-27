#include "ftp.h"

int tcpClientInit(int* sfd,char* ip,char* port)
{   
    int socketFd=socket(AF_INET,SOCK_STREAM,0);                                                
    ERROR_CHECK(socketFd,-1,"socket");
    struct sockaddr_in ser;
    bzero(&ser,sizeof(ser));
    ser.sin_family=AF_INET;
    ser.sin_port=htons(atoi(port));
    ser.sin_addr.s_addr=inet_addr(ip);
    int ret=connect(socketFd,(struct sockaddr *)&ser,sizeof(ser));
    ERROR_CHECK(ret,-1,"connect");
    *sfd=socketFd;
    return 0;
}

