#include "ftp.h"

void Cd(int newFd,pcmd_t pc,char* username)//返回当前的code
{
    
    char dir[MAXSIZE];
    bzero(dir,sizeof(dir));
    sscanf(pc->cmdBuf+3,"%s",dir);
    chdir(dir);
    bzero(dir,sizeof(dir));
    getcwd(dir,sizeof(dir));
    char currentDir[MAXSIZE];
    char dirTmp[MAXSIZE]="/home/vigoroushui/baiduDisk_2.0/server/src";
    bzero(currentDir,sizeof(currentDir));
    sprintf(currentDir,"%s%s","/home/vigoroushui/baiduDisk_2.0/server/src/",username);
    puts(dir);
    puts(currentDir);
    if(0==strcmp(dir,dirTmp))
    {  
        char errorInfo[50]="please operate in your own dir";
        int len=strlen(errorInfo);
        send(newFd,&len,sizeof(int),0);
        sendCycle(newFd,errorInfo,len);
        chdir(currentDir);
    }else{
        int len=strlen(dir);
        send(newFd,&len,sizeof(int),0);
        sendCycle(newFd,dir,len);
    }
}
void doMkdir(MYSQL* connect,int newFd,int code,char* userName,pcmd_t pc)
{   
  
    char mkdirQuery[200]={0};
    sprintf(mkdirQuery,"insert into virtualFileSystem(precode,fileName,owner,fileSize,fileType)\
    values(%d,'%s','%s','4096','d')",code,pc->cmdBuf+6,userName);
    puts(mkdirQuery);
    mysql_query(connect,mkdirQuery);
    printf("mkdir successful\n");
}
int doLs(MYSQL *connect,int code,int newFd)//加参数pcmd_t pc，可以实现ls filename的效果（还未实现）
{
    char lsQuery[200]={0};
    sprintf(lsQuery,"select fileType,fileName,fileSize from virtualFileSystem where precode=%d",code);
    mysql_query(connect,lsQuery);
    MYSQL_RES *res=mysql_use_result(connect);
    MYSQL_ROW row;
    if(res)
    {
        while((row=mysql_fetch_row(res))!=NULL)
        {
            printf("%s %s %s\n",row[0],row[1],row[2]);
        }
    }else{
        printf("nothing in this dir\n");
    }
    return 0;
}
            
int doGets(int newFd,pcmd_t pc)
{
    char fileName[256]={0};
    sscanf(pc->cmdBuf+5,"%s",fileName);
    int flag,ret;
    if(0==isFileExist(fileName))
    {
        struct stat s;
        bzero(&s,sizeof(s));
        ret=stat(fileName,&s);
        ERROR_CHECK(ret,-1,"stat");
        if(s.st_size<MMAPSIZE)
        {
            flag=0;
            sendCycle(newFd,(char*)&flag,sizeof(int));
            sendFile(newFd,fileName);
        }else{
            flag=0;
            sendCycle(newFd,(char*)&flag,sizeof(int));
            mmapSendFile(newFd,fileName);
        }
    }else{
        flag=-1;
        send(newFd,&flag,sizeof(int),0);
    }
    return 0;
}
int doPuts(int newFd,pcmd_t pc)//问题：直接上传会有stack mashing报错，断点上传即没问题
{
    int flag;
    recv(newFd,&flag,sizeof(int),0);
    if(0==flag)
    {
        recvFile(newFd);
    }
    if(-1==flag)
    {
        printf("%s exist!,please upload another one!\n",pc->cmdBuf+5);
    }
    return 0;
}
int doRemove(int newFd,pcmd_t pc)//最好添加提示信息，否则文件夹下的内容直接删除掉了。
{   
    char rmName[256]={0};
    sscanf(pc->cmdBuf+7,"%s",rmName);
    int flag;
    if(0==isFileExist(rmName))
    {
        char cmd[256]={0};
        sprintf(cmd,"rm -rf %s",rmName);
        system(cmd);
        flag=0;
        send(newFd,&flag,sizeof(int),0);
    }
    if(-1==isFileExist(rmName))
    {
        flag=-1;
        send(newFd,&flag,sizeof(int),0);
    }
    return 0;
}
void doPwd(int newFd)
{
    char* buf=getcwd(NULL,0);
    int bufLen=strlen(buf);
    send(newFd,&bufLen,sizeof(int),0);
    sendCycle(newFd,buf,bufLen);
}

