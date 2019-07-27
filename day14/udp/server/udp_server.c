#include <func.h>

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd=socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(socketFd,-1,"socket");
    struct sockaddr_in serAddr;
    serAddr.sin_family=AF_INET;
    serAddr.sin_port=htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr=inet_addr(argv[1]);
    int ret=bind(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"bind");
    struct sockaddr_in client;
    bzero(&client,sizeof(client));
    socklen_t fromLen=sizeof(struct sockaddr);
    char buf[1024]={0};
    recvfrom(socketFd,buf,sizeof(buf),0,(struct sockaddr*)&client,&fromLen);
    printf("gets %s client ip=%s,port%d\n",buf,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
    sendto(socketFd,"I am coming",11,0,(struct sockaddr*)&client,sizeof(client));
    close(socketFd);
    return 0;
}

