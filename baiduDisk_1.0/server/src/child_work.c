#include "ftp.h"

void doCd(int newFd,pcmd_t pc)//登录模块完成后，加一个传入参数：用户名
{
    char dir[MAXSIZE];
    bzero(dir,sizeof(dir));
    sscanf(pc->cmdBuf+3,"%s",dir);
    chdir(dir);
    bzero(dir,sizeof(dir));
    getcwd(dir,sizeof(dir));
    if(0==strcmp(dir,"/home/vigoroushui/baiduDisk_1.0/server"))
    {  
        char errorInfo[50]="please operate in your own dir";
        int len=strlen(errorInfo);
        send(newFd,&len,sizeof(int),0);
        sendCycle(newFd,errorInfo,len);
        chdir("/home/vigoroushui/baiduDisk_1.0/server/src");
    }else{
        int len=strlen(dir);
        send(newFd,&len,sizeof(int),0);
        sendCycle(newFd,dir,len);
    }
}
void doMkdir(int newFd,pcmd_t pc)
{
    char mkdirName[256]={0};
    sscanf(pc->cmdBuf+6,"%s",mkdirName);
    //printf("%s\n",mkdirName);
    int flag;
    flag=mkdir(mkdirName,0777);
    send(newFd,&flag,sizeof(int),0);
}
int doLs(int newFd)//加参数pcmd_t pc，可以实现ls filename的效果（还未实现）
{                  //sprintf拼接时有字符串溢出的可能性。warning！ 
    char *pwd=getcwd(NULL,0);
    DIR *dir=opendir(pwd);
    ERROR_CHECK(dir,NULL,"opendir");
    struct dirent *p=NULL;
    struct stat buf;
    bzero(&buf,sizeof(buf));
    char infor[256]={0};
    char type;
    int ret,inforLen;
    while((p=readdir(dir))!=NULL)
    {
        if(strcmp(p->d_name,".")!=0&&strcmp(p->d_name,"..")!=0)
        {
            if(4==p->d_type)
            {
                type='d';
            }
            if(8==p->d_type)
            {
                type='-';
            }
            sprintf(infor,"%s%s%s",pwd,"/",p->d_name);
            ret=stat(infor,&buf);
            ERROR_CHECK(ret,-1,"stat");
            bzero(infor,sizeof(infor));
            sprintf(infor,"%c %s %6ld\n",type,p->d_name,buf.st_size);
            inforLen=strlen(infor);
            send(newFd,&inforLen,sizeof(int),0);
            sendCycle(newFd,infor,inforLen);
            bzero(infor,sizeof(infor));
        }
    }
    int end=0;
    send(newFd,&end,sizeof(int),0);
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

//void getSalt(char* salt,char* passwd)
//{
//    int i,j;
//    for(i=0,j=0;passwd[i]&&j!=3;i++)
//    {
//        if('$'==passwd[i])
//        {   
//            j++;
//        }
//        strncpy(salt,passwd,i-1);
//    }
//}

int childWork(int newFd,int logFd,struct sockaddr_in client)
{
    // train_t userVer,pwdVer;
    // bzero(&userVer,sizeof(userVer));
    // bzero(&pwdVer,sizeof(pwdVer));
    // recv(newFd,&userVer.dataLen,sizeof(int),0);
    // recvCycle(newFd,userVer.buf,userVer.dataLen);
    // recv(newFd,&pwdVer.dataLen,sizeof(int),0);
    // recvCycle(newFd,pwdVer.buf,pwdVer.dataLen);
    // struct spwd *sp;
    // int flag;
    // if(NULL==(sp=getspnam(userVer.buf)))
    // {   
    //     flag=-1;
    //     send(newFd,&flag,sizeof(int),0);
    //     return 0;
    // }
    // char salt[512]={0};
    // getSalt(salt,sp->sp_pwdp);
    // if(strcmp(sp->sp_pwdp,crypt(pwdVer.buf,salt))!=0)
    // {
    //     flag=-2;
    //     send(newFd,&flag,sizeof(int),0);
    //     //return 0;
    // }else{
    //     flag=0;
    //     send(newFd,&flag,sizeof(int),0);
    //    int flag;
    //    int loginCf=loginVerify(newFd);
    //    if(-1==loginCf)
    //    {
    //        flag=-1;
    //        send(newFd,&flag,sizeof(int),0);
    //    }else{
    //        flag=0;
    //        send(newFd,&flag,sizeof(int),0);
    key_t semid=semget((key_t)KEY,1,IPC_CREAT|0600);
    struct sembuf sop[2];
    bzero(&sop[0],sizeof(sop[0]));
    bzero(&sop[1],sizeof(sop[1]));
    sop[0].sem_num=0;
    sop[0].sem_op=1;
    sop[0].sem_flg=SEM_UNDO;//v操作

    sop[1].sem_num=0;
    sop[1].sem_op=-1;
    sop[1].sem_flg=SEM_UNDO;//p操作

    char logBuf[1024]={0};
    pcmd_t pc=(pcmd_t)calloc(1,sizeof(cmd_t));
    //sprintf(logBuf,"client IP=%s, port=%d\n%s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),"client login successful!");
    //semop(semid,(struct sembuf*)&sop[1],1);
    //logWrite(logFd,logBuf);
    //semop(semid,(struct sembuf*)&sop[0],1);
    char loginInfo[100]="client login successful!\n";
    logOption(logBuf,logFd,semid,sop,loginInfo,client);
    int ret;
    while(1)
    {
        bzero(pc,sizeof(cmd_t));
        ret=recv(newFd,&pc->cmdLen,sizeof(int),0);
        if(0==ret||0==pc->cmdLen)
        {
            bzero(logBuf,sizeof(logBuf));
            //‘%s’ directive writing up to 1023 bytes into a region of size 997
            //sprintf(logBuf,"client IP=%s, port=%d\n%s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),"client exit successful!");
            //semop(semid,(struct sembuf*)&sop[1],1);
            //logWrite(logFd,logBuf);
            //semop(semid,(struct sembuf*)&sop[0],1);
            sprintf(loginInfo,"%s","client exit successful!\n");
            logOption(logBuf,logFd,semid,sop,loginInfo,client);
            printf("client exit!(IP=%s, port=%d)\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
            free(pc);
            pc=NULL;
            return 0;
        }
        recvCycle(newFd,pc->cmdBuf,pc->cmdLen);
        if(0==strncmp("cd",pc->cmdBuf,2))
        {   
            logOption(logBuf,logFd,semid,sop,pc->cmdBuf,client);
            doCd(newFd,pc);
        }
        if(0==strncmp("mkdir",pc->cmdBuf,5))
        {
            logOption(logBuf,logFd,semid,sop,pc->cmdBuf,client);
            doMkdir(newFd,pc);
        }
        if(0==strncmp("ls",pc->cmdBuf,2))
        {   
            logOption(logBuf,logFd,semid,sop,pc->cmdBuf,client);
            doLs(newFd);
        }
        if(0==strncmp("gets",pc->cmdBuf,4))
        {
            logOption(logBuf,logFd,semid,sop,pc->cmdBuf,client);
            doGets(newFd,pc);
        }
        if(0==strncmp("puts",pc->cmdBuf,4))
        {
            logOption(logBuf,logFd,semid,sop,pc->cmdBuf,client);
            doPuts(newFd,pc);
        }
        if(0==strncmp("remove",pc->cmdBuf,6))
        {
            logOption(logBuf,logFd,semid,sop,pc->cmdBuf,client);
            doRemove(newFd,pc);
        }
        if(0==strncmp("pwd",pc->cmdBuf,3))
        {
            logOption(logBuf,logFd,semid,sop,pc->cmdBuf,client);
            doPwd(newFd);
        }
    }
    //   }
    return 0;
}

