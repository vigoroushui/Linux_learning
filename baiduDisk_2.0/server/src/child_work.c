#include "ftp.h"

void doCd(int newFd,pcmd_t pc,char* username)//更换至对应的/home/用户名目录下，否则执行cd ..会一直后退
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
void doMkdir(int newFd,pcmd_t pc)
{
    char mkdirName[256]={0};
    sscanf(pc->cmdBuf+6,"%s",mkdirName);
    int flag;
    flag=mkdir(mkdirName,0777);
    send(newFd,&flag,sizeof(int),0);
}
int doLs(int newFd)//加参数pcmd_t pc，可以实现ls filename的效果
{                   
    char *pwd=getcwd(NULL,0);
    DIR *dir=opendir(pwd);
    ERROR_CHECK(dir,NULL,"opendir");
    struct dirent *p=NULL;
    struct stat buf;
    bzero(&buf,sizeof(buf));
    char infor[512]={0};
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

int childWork(int newFd,int logFd,struct sockaddr_in client)
{   
    int select;
    while(1)
    {
        recv(newFd,&select,sizeof(int),0);
        if(1==select)//注册账户，成功即为其创建一个以其名字命名的专属目录，用户所有操作只能在自己的目录中执行。
        {
            char *username=(char*)calloc(1,sizeof(char));
            bzero(username,sizeof(char));
            int flagRegister=loginRegister(newFd,username);
            if(0==flagRegister)
            {
                continue;
            }
            if(1==flagRegister)
            {
                mkdir(username,0777);
                continue;
            }
        }
        if(2==select)//登录账户，成功则可进入对应目录操作
        {   
            char *username=(char*)calloc(1,sizeof(char));
            bzero(username,sizeof(char));
            int flagVerify=loginVerify(newFd,username);
            puts(username);
            if(0==flagVerify)
            {
                continue;
            }
            if(1==flagVerify)
            {
                chdir(username);
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
                char loginInfo[100]="client login successful!\n";
                logOption(logBuf,logFd,semid,sop,loginInfo,client);//写入客户端操作日志的函数
                int ret;
                while(1)
                {
                    bzero(pc,sizeof(cmd_t));
                    ret=recv(newFd,&pc->cmdLen,sizeof(int),0);
                    if(0==ret||0==pc->cmdLen)
                    {
                        bzero(logBuf,sizeof(logBuf));
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
                        doCd(newFd,pc,username);
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
            }
        }else{
            continue;
        }
        return 0;
    }
}
