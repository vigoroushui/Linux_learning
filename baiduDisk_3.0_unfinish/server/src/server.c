#include "ftp.h"
//void sigExitFunc(int sigNum)
//{
//    write(exitFds[1],&sigNum,1);
//}
static key_t semid;
void sigHandle1(int signum)
{
    semctl(semid,0,IPC_RMID,NULL);
    exit(0);
}
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2);
    key_t semid=semget((key_t)KEY,1,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    int ret=semctl(semid,0,SETVAL,1);
    ERROR_CHECK(ret,-1,"semctl");
    struct sembuf sop[2];
    bzero(&sop[0],sizeof(sop[0]));
    bzero(&sop[1],sizeof(sop[1]));
    sop[0].sem_num=0;
    sop[0].sem_op=1;
    sop[0].sem_flg=SEM_UNDO;//v操作
    sop[1].sem_num=0;
    sop[1].sem_op=-1;
    sop[1].sem_flg=SEM_UNDO;//p操作
    signalInit();

    char ip[32]={0};
    char port[5]={0};
    read_conf(argv[1],ip,port);

    int logFd=open("log",O_CREAT|O_WRONLY|O_APPEND,0600);
    ERROR_CHECK(logFd,-1,"open");
    int processNum=PROCESSNUM;
    int socketFd;
    tcpInit(&socketFd,ip,port);
    int epfd=epoll_create(1);
    struct epoll_event *evs;
    evs=(struct epoll_event*)calloc(processNum+1,sizeof(struct epoll_event));
    epollInAdd(epfd,socketFd);

    process_data_t *pData=(process_data_t*)calloc(processNum,sizeof(process_data_t));
    makeChild(pData,processNum,logFd);//创建子进程
    int i;
    for(i=0;i<processNum;i++)
    {
        epollInAdd(epfd,pData[i].pipeFd);
    }
    struct sockaddr_in client;
    memset(&client,0,sizeof(client));
    socklen_t addrlen=sizeof(client);
    int readyFdCount,newFd,j;
    int noBusyFlag;
    char logBuf[256]={0};
    while(1)
    {
        memset(evs,0,(processNum+1)*sizeof(evs));
        readyFdCount=epoll_wait(epfd,evs,processNum+1,-1);
        for(i=0;i<readyFdCount;i++)
        {
            if(evs[i].data.fd==socketFd)
            {   
                newFd=accept(socketFd,(struct sockaddr*)&client,&addrlen);//pointer targets in passing argument 3 of ‘accept’ differ in signedness
                bzero(logBuf,sizeof(logBuf));
                sprintf(logBuf,"client IP=%s,port=%d\n%s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),"client login");
                logWrite(logFd,logBuf);
                for(j=0;j<processNum;j++)
                {
                    if(0==pData[j].busy)
                    {
                        sendFd(pData[j].pipeFd,newFd);
                        write(pData[j].pipeFd,&client,sizeof(client));
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
                    read(pData[j].pipeFd,&noBusyFlag,sizeof(int));
                    pData[j].busy=0;
                    //printf("%d pid is not busy\n",pData[j].pid);
                    break;
                }
            }
        }
    }
    return 0;
}

