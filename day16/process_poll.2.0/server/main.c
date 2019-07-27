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
    evs=(struct epoll_event*)calloc(processNum+1,sizeof(struct epoll_event));//申请监控描述符的事件空间
    epollInAdd(epfd,socketFd);//监控读事件
    // register moniter the opposite terminal of all the child processes 
    for(i=0;i<processNum;i++)
    {
        epollInAdd(epfd,pData[i].pipeFd);
    }
    int readyFdCount,newFd,j;//就绪的描述符数目
    char noBusyflag;
    while(1)
    {
        readyFdCount=epoll_wait(epfd,evs,processNum+1,-1);
        for(i=0;i<readyFdCount;i++)
        {
            if(evs[i].data.fd==socketFd)//有客户端连接，accept，并找非忙碌子进程
            {
                newFd=accept(socketFd,NULL,NULL);// receive the request of client
                for(j=0;j<processNum;j++)// find leisure child process
                {
                    if(0==pData[j].busy)//可以加入一个队列，增强公平性
                    {
                        sendFd(pData[j].pipeFd,newFd);// send task to the target child process
                        pData[j].busy=1;//set the status of child process -> busy.
                        printf("%d pid is busy\n",pData[i].pid);
                        break;
                    }
                }
                close(newFd);//直接断开连接，可能存在引用计数
            }
            for(j=0;j<processNum;j++)//找该进程等于哪一个子进程的对端
            {
                if(evs[i].data.fd==pData[j].pipeFd)//从管道把数据读出来，不然一直处于可读状态
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

