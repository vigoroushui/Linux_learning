#include "process_pool.h"
// create child processes, and initialize main data struct
int makeChild(process_data_t *p,int processNum)
{   
    int i,fds[2],ret;
    pid_t pid;
    for(i=0;i<processNum;i++)
    {
        ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);//创建了processNum条全双工管道
        ERROR_CHECK(ret,-1,"socketpair");
        pid=fork();
        if(0==pid)
        {
            close(fds[0]);
            childHandle(fds[1]);
        }
        close(fds[1]);
        p[i].pid=pid;
        p[i].pipeFd=fds[0];//存储子进程管道对端
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
        recvFd(pipeFd,&newFd);//父进程给子进程发任务，接收任务，没有任务时，子进程睡觉
        tranFile(newFd);//给客户端发文件
        close(newFd);
        write(pipeFd,&finishFlag,1);//子进程告诉父进程，已经完成任务
    }
}
