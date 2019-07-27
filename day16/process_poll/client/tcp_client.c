#include <func.h>
int recvCycle(int,void*,int);
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(socketFd,-1,"socket");
    struct sockaddr_in serAddr;
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family=AF_INET;
    serAddr.sin_port=htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr=inet_addr(argv[1]);
    int ret=connect(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"connect");
    int fd;
    int dataLen;
    char buf[1000]={0};
    recvCycle(socketFd,&dataLen,4);
    //receive the filename
    recvCycle(socketFd,buf,dataLen);
    fd=open(buf,O_CREAT|O_RDWR,0666);
    ERROR_CHECK(fd,-1,"open");
    //receive the filesize
    off_t fileSize,downLoadSize=0;
    recvCycle(socketFd,&dataLen,4);
    recvCycle(socketFd,&fileSize,dataLen);
    // printf("fileSize=%ld\n",fileSize);
    time_t lastTime,nowTime;
    lastTime=nowTime=time(NULL);
    struct timeval start,end;
    gettimeofday(&start,NULL);
    while(1)
    {
        recvCycle(socketFd,&dataLen,4);
        if(dataLen>0)
        {
            recvCycle(socketFd,buf,dataLen);
            write(fd,buf,dataLen);
            downLoadSize+=dataLen;
            time(&nowTime);
           // printf("test1\n");
            if(nowTime-lastTime>=1)
            {
                printf("%5.2f%s\r",(float)downLoadSize/fileSize*100,"%");
             //   printf("test2\n");
                fflush(stdout);
                lastTime=nowTime;
            }
        }else{
            printf("100.00%%\n");
            break;
        }
        // ftruncate(fd,fileSize);
        // char *pMap=(char*)mmap(NULL,fileSize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        // ERROR_CHECK(pMap,(char*)-1,"mmap");
        // recvCycle(socketFd,pMap,fileSize);
        // munmap(pMap,fileSize);     
    }
    gettimeofday(&end,NULL);
    printf("use time=%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    close(fd);
    close(socketFd);
    return 0;
}

