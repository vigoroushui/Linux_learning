#include <func.h>
#include "tcp_net_socket.h"

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd=tcp_init(argv[1],atoi(argv[2]));
    fd_set rdset;
    fd_set needMoniterFd;
    int ret;
    int newFd;
    char buf[1024];
    FD_ZERO(&needMoniterFd);
    FD_SET(STDIN_FILENO,&needMoniterFd);
    FD_SET(socketFd,&needMoniterFd);
    while(1)
    {
        memcpy(&rdset,&needMoniterFd,sizeof(fd_set));
        ret=select(14,&rdset,NULL,NULL,NULL);
        if(ret>0)
        {
            if(FD_ISSET(socketFd,&rdset))
            {
                newFd=accept(socketFd,NULL,NULL);
                ERROR_CHECK(newFd,-1,"accept");
                FD_SET(newFd,&needMoniterFd);
            }
            if(FD_ISSET(STDIN_FILENO,&rdset))
            {
                bzero(buf,sizeof(buf));
                ret=read(STDIN_FILENO,buf,sizeof(buf));
                if(0==ret)
                {
                    printf("goodbye\n");
                    break;
                }
                send(newFd,buf,strlen(buf)-1,0);
            }
            if(FD_ISSET(newFd,&rdset))
            {
                bzero(buf,sizeof(buf));
                ret=recv(newFd,buf,sizeof(buf),0);
                if(0==ret)
                {   
                    printf("client is over\n");
                    FD_CLR(newFd,&needMoniterFd);
                    close(newFd);
                }else{
                    printf("%s\n",buf);
                }
            }
        }
    }
    return 0;
}

    
