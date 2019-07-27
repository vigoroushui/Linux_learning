#include "ftp.h"

int childWork(int newFd,int logFd,struct sockaddr_in client)
{   
    char server[10]="localhost";
    char user[5]="root";
    char password[4]="xyh";
    char database[10]="baiduDisk";
    MYSQL *connect;
    connect=mysql_init(NULL);
    if(!mysql_real_connect(connect,server,user,password,database,0,NULL,0))
    {   
        printf("Error connecting to database:%s\n",mysql_error(connect));
    }else{
        printf("connected...\n");
    }
    int select;
    while(1)
    {
        recv(newFd,&select,sizeof(int),0);
        if(1==select)
        {
            char *username=(char*)calloc(1,sizeof(char));
            bzero(username,sizeof(char));
            int flagRegister=loginRegister(newFd,username,connect);
            printf("flagRegister=%d\n",flagRegister);
            if(0==flagRegister)
            {
                continue;
            }
            if(1==flagRegister)
            {
                char insertDir[200]="insert into virtualFileSystem(fileName,owner,fileSize,fileType) values(";
                sprintf(insertDir,"%s'%s','%s','4096','d');",insertDir,username,username);
                puts(insertDir);
                mysql_query(connect,insertDir);//有问题，mysql中执行能插入，放这里却插入不进去
                printf("insert successful\n");
                continue;
            }
        }
        if(2==select)
        {   
            char *username=(char*)calloc(1,sizeof(char));
            bzero(username,sizeof(char));
            int flagVerify=loginVerify(newFd,username,connect);
            printf("flagVerify=%d\n",flagVerify);
            if(0==flagVerify)
            {
                continue;
            }
            if(1==flagVerify)
            {
                childWorkAgain();
            }
        }
    }
    return 0;
}
void childWorkAgain(MYSQL *connect,char * username)
{
    char dirInit[200]={0};
    sprintf(dirInit,"select min(code) from virtualFileSystem where name='%s'",username);
    mysql_query(connect,dirInit);
    
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
    logOption(logBuf,logFd,semid,sop,loginInfo,client);
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
}
