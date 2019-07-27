#include "ftp.h"

int makeChild(process_data_t *p,int processNum,int logFd)
{
    pid_t pid;
    int i;
    int fds[2];
    int ret;
    for(i=0;i<processNum;i++)
    {
        ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        ERROR_CHECK(ret,-1,"socketpair");
        pid=fork();
        if(0==pid)
        {
            close(fds[0]);
            childHandle(fds[1],logFd);
        }
        close(fds[1]);
        p[i].pid=pid;
        p[i].pipeFd=fds[0];
        p[i].busy=0;
    }
    return 0;
}

int childHandle(int pipeFd,int logFd)
{   
    int newFd;
    struct sockaddr_in client;
    int finishFlag=1;
    while(1)
    {   
        //printf("child is waiting\n");
        recvFd(pipeFd,&newFd);
        //if(childExitFlag)
        //{
        //    exit(0);
        //}
        //printf("is ka zaizhege difang ?\n");
        //tranFile(newFd);
        read(pipeFd,&client,sizeof(client));
        childWork(newFd,logFd,client);
        close(newFd);
        write(pipeFd,&finishFlag,sizeof(int));
    }
}
