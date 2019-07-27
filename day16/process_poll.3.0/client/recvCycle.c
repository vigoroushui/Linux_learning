#include<func.h>

int recvCycle(int sfd,void* buf,int recvLen)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<recvLen)
    {
        ret=recv(sfd,p+total,recvLen-total,0);
        if(0==ret)
        {
            return -1;
        }
        total+=ret;
    }
    return 0;
}
int recvCycle_print_per(int sfd,void* buf,int recvLen)
{
    char *p=(char*)buf;//每次偏移1字节
    int total=0,ret;//total就是downLoadSize
    time_t lastTime,nowTime;
    lastTime=nowTime=time(NULL);
    while(total<recvLen)
    {
        ret=recv(sfd,p+total,recvLen-total,0);
        total+=ret;
        nowTime=time(NULL);
        if(nowTime-lastTime>=1)
        {
            printf("%5.2f%s\r",(float)total/recvLen*100,"%");
            fflush(stdout);
            lastTime=nowTime;
        }
    }
    printf("100.0%%\n");
    return 0;
}
