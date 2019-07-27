#include <func.h>

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketfd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(socketfd,-1,"socket");
    struct sockaddr_in serAddr;
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family=AF_INET;
    serAddr.sin_port=htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr=inet_addr(argv[1]);
    int ret=bind(socketfd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"bind");
    listen(socketfd,10);//10为某一瞬间并发的链接数，最大设置128
    struct sockaddr_in client;
    bzero(&client,sizeof(client));
    socklen_t clientLen=sizeof(client);
    int newfd=accept(socketfd,(struct sockaddr*)&client,&clientLen);
    printf("client ip=%s,client port=%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
    char buf[1024]={0};
    recv(newfd,buf,sizeof(buf),0);
    printf("server gets=%s\n",buf);
    send(newfd,"hello client!",13,0);
    while(1);
    return 0;
}

