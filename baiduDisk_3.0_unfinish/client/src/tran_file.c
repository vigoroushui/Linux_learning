#include "ftp.h"

int isFileExist(char* fileName)
{
    char* pwd=getcwd(NULL,0);
    DIR* dir=opendir(pwd);
    ERROR_CHECK(dir,NULL,"opendir");
    struct dirent *p=NULL;
    while((p=readdir(dir))!=NULL)
    {
        if(0==strcmp(p->d_name,fileName))
        {
            //printf("file exist!please operate!\n");
            return 0;
        }
    }
    free(pwd);
    pwd=NULL;
    return -1;
}
int isFileExistTmp(char* fileName)
{
    char* pwd=getcwd(NULL,0);
    DIR* dir=opendir(pwd);
    ERROR_CHECK(dir,NULL,"opendir");
    char tmp[256]={0};
    strcpy(tmp,fileName);
    strcat(tmp,".avi");
    struct dirent *p=NULL;
    while((p=readdir(dir))!=NULL)
    {
        if(0==strcmp(p->d_name,tmp))
        {
            //printf("File exists, not all transferred!\n");
            return 1;
        }
    }
    free(pwd);
    pwd=NULL;
    return 0;
}
int recvFile(int socketFd)
{
    char fileName[256]={0};
    int fileLen;
    recvCycle(socketFd,(char *)&fileLen,sizeof(int));
    recvCycle(socketFd,fileName,fileLen);
    if(1==isFileExistTmp(fileName))
    {
        char fileTmp[256]={0};
        sprintf(fileTmp,"%s%s",fileName,".avi");
        int fd=open(fileTmp,O_RDWR);
        ERROR_CHECK(fd,-1,"open");
        struct stat s;
        bzero(&s,sizeof(s));
        fstat(fd,&s);
        sendCycle(socketFd,(char*)&s.st_size,sizeof(int));
        lseek(fd,s.st_size,SEEK_SET);
        printf("file downloaded %ld,keep downloading!\n",s.st_size);
        train_t data;
        while(1)
        {
            bzero(&data,sizeof(data));
            recvCycle(socketFd,(char*)&data.dataLen,sizeof(int));
            if(0==data.dataLen)
            {
                break;
            }
            recvCycle(socketFd,(char*)data.buf,data.dataLen);
            write(fd,data.buf,data.dataLen);
        }
        close(fd);
        char cmd[128]={0};
        sprintf(cmd,"%s %s %s","mv",fileTmp,fileName);
        system(cmd);
        printf("\"%s\" downloading successful!\n",fileName);
    }else{
        char fileTmp[256]={0};
        sprintf(fileTmp,"%s%s",fileName,".avi");
        int fd=open(fileTmp,O_CREAT|O_WRONLY,0666);
        ERROR_CHECK(fd,-1,"open");
        printf("downloading now!\n");
        int fileLen=0;
        sendCycle(socketFd,(char*)&fileLen,sizeof(int));
        train_t data;
        while(1)
        {   
            bzero(&data,sizeof(data));
            recvCycle(socketFd,(char*)&data.dataLen,sizeof(int));
            if(0==data.dataLen)
            {   
                break;
            }   
            recvCycle(socketFd,data.buf,data.dataLen);
            write(fd,data.buf,data.dataLen);
        }   
        close(fd);
        char cmd[128]={0};
        sprintf(cmd,"%s %s %s","mv",fileTmp,fileName);
        //printf("%s\n",cmd);
        system(cmd);
        printf("\"%s\" download successful!\n",fileName);
    }
    return 0;
}
int sendFile(int socketFd,char* fileName)
{
    int fileLen=strlen(fileName);
    sendCycle(socketFd,(char*)&fileLen,sizeof(int));
    sendCycle(socketFd,fileName,fileLen);
    int fd=open(fileName,O_RDONLY);
    ERROR_CHECK(fd,-1,"open");
    int fileSizeOffset;
    recvCycle(socketFd,(char*)&fileSizeOffset,sizeof(int));
    int ret=lseek(fd,fileSizeOffset,SEEK_SET);
    ERROR_CHECK(ret,-1,"lseek");
    printf("sending file!\n");
    train_t data;
    while(bzero(&data,sizeof(data)),(data.dataLen=read(fd,data.buf,sizeof(data.buf)))>0)
    {
        sendCycle(socketFd,(char*)&data.dataLen,sizeof(int));
        sendCycle(socketFd,data.buf,data.dataLen);
    }
    int flag=0;
    sendCycle(socketFd,(char*)&flag,sizeof(int));
    close(fd);
    printf("send \"%s\" successful!\n",fileName);
    return 0;
}
int mmapSendFile(int socketFd,char* fileName)
{
    
    int fileLen=strlen(fileName);
    sendCycle(socketFd,(char*)&fileLen,sizeof(int));
    sendCycle(socketFd,fileName,fileLen);
    int fd=open(fileName,O_RDONLY);
    ERROR_CHECK(fd,-1,"open");
    int fileSizeOffset;
    recvCycle(socketFd,(char*)&fileSizeOffset,sizeof(int));
    printf("mmap sending file\n");
    struct stat s;
    bzero(&s,sizeof(s));
    fstat(fd,&s);
    int fileSize=s.st_size;
    char *filePoint=(char*)mmap(NULL,fileSize,PROT_READ,MAP_SHARED,fd,0);
    if(MAP_FAILED==filePoint)
    {
        perror("mmap");
        return -1;
    }
    train_t data;
    bzero(&data,sizeof(data));
    data.dataLen=fileSize-fileSizeOffset;
    int tmp=sizeof(data.buf);
    while(data.dataLen>0)
    {
        if(data.dataLen>tmp)
        {
            sendCycle(socketFd,(char*)&tmp,sizeof(int));
            sendCycle(socketFd,filePoint+fileSizeOffset,tmp);
            data.dataLen-=tmp;
            fileSizeOffset+=tmp;
        }else{
            tmp=fileSize-fileSizeOffset;
            sendCycle(socketFd,(char*)&tmp,sizeof(int));
            sendCycle(socketFd,filePoint+fileSizeOffset,tmp);
            data.dataLen-=tmp;
            fileSizeOffset+=tmp;
        }
    }
    int flag=0;
    send(socketFd,&flag,sizeof(flag),0);
    printf("mmap send \"%s\"successful!",fileName);
    close(fd);
    return 0;
}
