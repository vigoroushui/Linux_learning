#include "process_pool.h"
#define DEBUG
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
#ifdef DEBUG
    int i;
    for(i=0;i<processNum;i++)
    {
        printf("pid=%d,pipeFd=%d\n",pData[i].pid,pData[i].pipeFd);
    }
#endif
    int socketFd;
    tcpInit(&socketFd,argv[1],argv[2]);//initialize socket and open listen terminal
    int epfd=epoll_create(1);
    struct epoll_event *evs;
    evs=(struct epoll_event*)calloc(processNum+1,sizeof(struct epoll_event));
    epollInAdd(epfd,socketFd);
    // register moniter the opposite terminal of all the child processes 
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
            if(evs[i].data.fd==socketFd)
            {
                newFd=accept(socketFd,NULL,NULL);// receive the request of client
                for(j=0;j<processNum;j++)// find leisure child process
                {
                    if(0==pData[j].busy)
                    {
                        sendFd(pData[j].pipeFd,newFd);// send task to the target child process
                        pData[j].busy=1;//set the status of child process -> busy.
                        printf("%d pid is busy\n",pData[i].pid);
                        break;
                    }
                }
                close(newFd);
            }
            for(j=0;j<processNum;j++)
            {
                if(evs[i].data.fd==pData[j].pipeFd)
                {
                    read(pData[j].pipeFd,&noBusyflag,1);//receive the inform of child process
                    pData[j].busy=0;//set the status of child process -> leisure
                    printf("%d pid is not busy\n",pData[j].pid);
                    break;
                }
            }
        }
    }
    return 0;
}

