#include "process_pool.h"
int exitFds[2];
void sigExitFunc(int sigNum)
{
    write(exitFds[1],&sigNum,1);
}
int main(int argc,char* argv[])
{
    if(argc!=4)
    {
        printf("./process_pool_server ip port process_num\n");
        return -1;
    }
    int processNum=atoi(argv[3]);
    process_data_t *pData=(process_data_t*)calloc(processNum,sizeof(process_data_t));
    makeChild(pData,processNum);
    //exit func
    pipe(exitFds);
    signal(SIGUSR1,sigExitFunc);
    int i;
#ifdef DEBUG
    for(i=0;i<processNum;i++)
    {
        printf("pid=%d pipeFd=%d\n",pData[i].pid,pData[i].pipeFd);
    }
#endif
    int socketFd;
    tcpInit(&socketFd,argv[1],argv[2]);
    int epfd=epoll_create(1);
    struct epoll_event *evs;
    evs=(struct epoll_event*)calloc(processNum+2,sizeof(struct epoll_event));
    epollInAdd(epfd,socketFd);
    epollInAdd(epfd,exitFds[0]);
    for(i=0;i<processNum;i++)
    {
        epollInAdd(epfd,pData[i].pipeFd);
    }
    int readyFdCount,newFd,j;
    char noBusyflag;
    while(1)
    {
        readyFdCount=epoll_wait(epfd,evs,processNum+1,-1);
        for(i=0;i<readyFdCount;i++)
        {
            if(evs[i].data.fd==exitFds[0])
            {
                for(j=0;j<processNum;j++)
                {
                    kill(pData[j].pid,SIGUSR1);
                }
                for(j=0;j<processNum;j++)
                {
                    wait(NULL);
                }
                printf("exit ok!\n");
                exit(0);
            }
            if(evs[i].data.fd==socketFd)
            {
                newFd=accept(socketFd,NULL,NULL);
                for(j=0;j<processNum;j++)
                {
                    if(0==pData[j].busy)
                    {
                        sendFd(pData[j].pipeFd,newFd);
                        pData[j].busy=1;
                        printf("%d pid is busy\n",pData[j].pid);
                        break;
                    }
                }
                close(newFd);
            }
            for(j=0;j<processNum;j++)
            {
                if(evs[i].data.fd==pData[j].pipeFd)
                {
                    read(pData[j].pipeFd,&noBusyflag,1);//收到子进程的通知
                    pData[j].busy=0;//子进程设置为非忙碌
                    printf("%d pid is not busy\n",pData[j].pid);
                    break;
                }
            }
        }
    }
    return 0;
}

