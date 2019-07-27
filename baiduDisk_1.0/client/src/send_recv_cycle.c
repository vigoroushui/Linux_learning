#include "ftp.h"

int recvCycle(int newFd,char* buf,int recvLen)
{   
    int total=0,ret;
    while(total<recvLen)
    {
        ret=recv(newFd,buf+total,recvLen-total,0);
        if(0==ret)
        {   
            return -1;
        }
        total+=ret;
    }       
    return 0;
}
int sendCycle(int newFd,char* buf,int sendLen)
{
    int total=0,ret;
    while(total<sendLen)
    {
        ret=send(newFd,buf+total,sendLen-total,0);
        if(0==ret)
        {
            return -1;
        }
        total+=ret;
    }
    return 0;
}
