#include "tcp_net_socket.h"

int tcp_init(const char*ip,int port)
{
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==sfd)
    {
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(port);
    serverAddr.sin_addr.s_addr=inet_addr(ip);
    int reuse=1;
    if(-1==setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int)))
    {
        perror("setsockopt");
        close(sfd);
        exit(-1);
    }

    if(-1==bind(sfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)))
    {
        perror("bind");
        close(sfd);
        exit(-1);
    }
    if(-1==listen(sfd,10))
    {
        perror("listen");
        close(sfd);
        exit(-1);
    }
    return sfd;
}

int tcp_accept(int sfd)
{
    struct sockaddr_in clientaddr;
    memset(&clientaddr,0,sizeof(clientaddr));
    socklen_t addrlen=sizeof(clientaddr);
    int new_fd=accept(sfd,(struct sockaddr*)&clientaddr,&addrlen);//sfd 接受客户端连接，并创建新的 socket 为 new_fd，将请求连接的客户端的 ip、port 保存在结构体 clientaddr 中
    if(-1==new_fd)
    {
        perror("error");
        close(sfd);
        exit(-1);
    }
    printf("%s %d success connect...\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
    return new_fd;
}

int tcp_connect(const char*ip,int port)
{
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==sfd)
    {
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(port);
    serveraddr.sin_addr.s_addr=inet_addr(ip);
    if(-1==connect(sfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)))
    {
        perror("connect");
        close(sfd);
        exit(-1);
    }
    return sfd;
}

void signalhandler(void)
{
    sigset_t sigSet;
    sigemptyset(&sigSet);
    sigaddset(&sigSet,SIGINT);
    sigaddset(&sigSet,SIGQUIT);
    sigprocmask(SIG_BLOCK,&sigSet,NULL);
}

