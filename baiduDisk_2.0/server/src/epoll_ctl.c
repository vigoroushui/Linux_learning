#include "ftp.h"

int epollInAdd(int epfd,int fd)//注册监听的事件类型
{   
    struct epoll_event event;
    bzero(&event,sizeof(event));
    event.events=EPOLLIN;
    event.data.fd=fd;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}

