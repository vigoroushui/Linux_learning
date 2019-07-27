#include "process_pool.h"
// create child processes, and initialize main data struct
int makeChild(process_data_t *p,int processNum)
{   
    int i,fds[2],ret;
    pid_t pid;
    for(i=0;i<processNum;i++)
    {
        ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        ERROR_CHECK(ret,-1,"socketpair");
        pid=fork();
        if(0==pid)
        {
            close(fds[0]);
            childHandle(fds[1]);
        }
        close(fds[1]);
        p[i].pid=pid;
        p[i].pipeFd=fds[0];
        p[i].busy=0;
    }
    return 0;
}

int childHandle(int pipeFd)
{
    int newFd;
    char finishFlag;
    while(1)
    {
        recvFd(pipeFd,&newFd);
        tranFile(newFd);
        close(newFd);
        write(pipeFd,&finishFlag,1);
    }
}
