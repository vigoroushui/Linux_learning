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
    // send the context of file
    //ret=sendfile(newFd,fd,NULL,buf.st_size);
    //ERROR_CHECK(ret,-1,"sendfile");
    while((train.dataLen=read(fd,train.buf,sizeof(train.buf))))
    {
        ret=send(newFd,&train,4+train.dataLen,0);
        if(-1==ret)
        {
            return -1;
        }
    }
    send(newFd,&train,4,0);
    return 0;
}

