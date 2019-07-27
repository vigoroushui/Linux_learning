#include "process_pool.h"
void sigFunc(int signum)
{
    printf("%d is coming\n",signum);
}

int tranFile(int newFd)
{
    signal(SIGPIPE,sigFunc);
    train_t train;
    int ret;
    // send the name of file
    train.dataLen=strlen(FILENAME);
    strcpy(train.buf,FILENAME);
    send(newFd,&train,4+train.dataLen,0);
    // send the size of file to client
    struct stat buf;
    int fd=open(FILENAME,O_RDWR);
    fstat(fd,&buf);
    train.dataLen=sizeof(buf.st_size);
    memcpy(train.buf,&buf.st_size,train.dataLen);
    send(newFd,&train,4+train.dataLen,0);
    //send the context of file
    char *pMap=(char*)mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(pMap,(char*)-1,"mmap");
    ret=send(newFd,pMap,buf.st_size,0);
    if(-1==ret)
    {
        return -1;
    }
    send(newFd,&train,4,0);
    return 0;
}

