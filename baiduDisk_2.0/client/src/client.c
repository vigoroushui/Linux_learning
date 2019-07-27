#include "ftp.h"

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(socketFd,-1,"socket");
    struct sockaddr_in ser;
    bzero(&ser,sizeof(ser));
    ser.sin_family=AF_INET;
    ser.sin_port=htons(atoi(argv[2]));
    ser.sin_addr.s_addr=inet_addr(argv[1]);
    int ret=connect(socketFd,(struct sockaddr *)&ser,sizeof(ser));
    ERROR_CHECK(ret,-1,"connect");
    while(1)
    {   
        system("clear");
        printf("please select a way to operate:\n");
        printf("1.Register an account\n");
        printf("2.log in\n");
        int select;
        scanf("%d",&select);
        send(socketFd,&select,sizeof(int),0);
        //system("clear");
        if(1==select)//注册
        {   
            system("clear");
            char userName[30]={0};
            printf("please input username:");
            scanf("%s",userName);
            int nameLen=strlen(userName);
            send(socketFd,&nameLen,sizeof(int),0);
            sendCycle(socketFd,userName,nameLen);
            int flags;
            recv(socketFd,&flags,sizeof(int),0);
            if(-1==flags)
            {   
                printf("this user exist\n!");
                continue;
            }
            if(0==flags)
            {   
                char salt[20]={0};
                int saltLen;
                recv(socketFd,&saltLen,sizeof(int),0);
                recvCycle(socketFd,salt,saltLen);
                //printf("%s\n",salt);
                char *userPassword;
                bzero(&userPassword,sizeof(userPassword));
                userPassword=getpass("please input password:");
                char cryotogram[256]={0};
                bzero(&cryotogram,sizeof(cryotogram));
                strcpy(cryotogram,crypt(userPassword,salt));//给输入的密码加点盐，即变成密文。

                int cryptLen=strlen(cryotogram);
                send(socketFd,&cryptLen,sizeof(int),0);
                sendCycle(socketFd,cryotogram,cryptLen);//发送密文

                char info[30]={0};
                int infoLen;
                recv(socketFd,&infoLen,sizeof(int),0);
                recvCycle(socketFd,info,infoLen);//接受注册信息
                printf("%s",info);
                //goto target1;
                continue;
            }
        }
        else if(2==select)//登录
        {        
            system("clear");
            char userName[30]={0};
            printf("please input username:");
            scanf("%s",userName);
            int nameLen=strlen(userName);
            send(socketFd,&nameLen,sizeof(int),0);
            sendCycle(socketFd,userName,nameLen);
            //puts(userName);
            int flags;
            recv(socketFd,&flags,sizeof(int),0);
            if(-1==flags)
            {   
                printf("this user is not exist!\n");
                continue;
            }
            if(0==flags)
            {   
                char salt[30]={0};
                int saltLen;
                recv(socketFd,&saltLen,sizeof(int),0);
                recvCycle(socketFd,salt,saltLen);
                //printf("%s\n",salt);
                char *userPassword;
                bzero(&userPassword,sizeof(userPassword));
                userPassword=getpass("please input password:");
                system("clear");
                char cryotogram[512]={0};
                bzero(&cryotogram,sizeof(cryotogram));
                strcpy(cryotogram,crypt(userPassword,salt));//给输入的密码加点盐，即变成密文。

                int cryptLen=strlen(cryotogram);
                send(socketFd,&cryptLen,sizeof(int),0);
                sendCycle(socketFd,cryotogram,cryptLen);//发送密文

                char info[30]={0};
                int infoLen;
                recv(socketFd,&infoLen,sizeof(int),0);
                recvCycle(socketFd,info,infoLen);//接受登录信息
                printf("%s",info);

                pdata_t pd=(pdata_t)calloc(1,sizeof(data_t));
                pcmd_t pc=(pcmd_t)calloc(1,sizeof(cmd_t));
                int readLen;
                while(bzero(pc,sizeof(cmd_t)),fflush(stdin),(readLen=read(0,pc->cmdBuf,sizeof(pc->cmdBuf)))>0)
                {
                    pc->cmdLen = strlen(pc->cmdBuf);
                    send(socketFd,&pc->cmdLen,sizeof(int),0);
                    sendCycle(socketFd,pc->cmdBuf,pc->cmdLen);
                    if(0==strncmp("cd",pc->cmdBuf,2))
                    {   
                        system("clear");
                        bzero(pd,sizeof(data_t));  
                        recv(socketFd,&pd->len,sizeof(int),0);
                        recvCycle(socketFd,pd->buf,pd->len);
                        printf("%s\n",pd->buf);
                    }
                    if(0==strncmp("mkdir",pc->cmdBuf,5))
                    {
                        system("clear");
                        int flag;
                        recv(socketFd,&flag,sizeof(int),0);
                        if(0==flag)
                        {
                            printf("mkdir %s success!\n",pc->cmdBuf+6);
                        }
                        if(-1==flag)
                        {
                            printf("mkdir fail!\n");
                        }
                    }
                    if(0==strncmp("ls",pc->cmdBuf,2))
                    {
                        system("clear");
                        while(1)
                        {
                            bzero(pd,sizeof(data_t));
                            recv(socketFd,&pd->len,sizeof(int),0);
                            if(0==pd->len)
                            {
                                break;
                            }
                            recvCycle(socketFd,pd->buf,pd->len);
                            printf("%s",pd->buf);
                        }
                    }
                    if(0==strncmp("gets",pc->cmdBuf,4))
                    {
                        system("clear");
                        int flag;
                        recv(socketFd,&flag,sizeof(int),0);
                        if(0==flag)
                        {
                            recvFile(socketFd);
                        }
                        if(-1==flag)
                        {
                            printf("%s is not exist!\n",pc->cmdBuf+5);
                        }
                    } 
                    if(0==strncmp("puts",pc->cmdBuf,4))
                    {   
                        system("clear");
                        char fileName[256]={0};
                        int flag;
                        sscanf(pc->cmdBuf+5,"%s",fileName);
                        if(0==isFileExist(fileName))
                        {
                            struct stat s;
                            bzero(&s,sizeof(s));
                            stat(fileName,&s);
                            if(s.st_size<MMAPSIZE)
                            {
                                flag=0;
                                send(socketFd,&flag,sizeof(int),0);
                                sendFile(socketFd,fileName);
                            }else{
                                flag=0;
                                send(socketFd,&flag,sizeof(int),0);
                                mmapSendFile(socketFd,fileName);
                            }
                        }else{
                            flag=1;
                            send(socketFd,&flag,sizeof(int),0);
                        }           
                    }
                    if(0==strncmp("remove",pc->cmdBuf,6))
                    {   
                        system("clear");
                        int flag;
                        recv(socketFd,&flag,sizeof(int),0);
                        if(0==flag)
                        {
                            printf("remove successful!\n");
                        }
                        if(-1==flag)
                        {
                            printf("target file is not exist!\n");
                        }
                    }
                    if(0==strncmp("pwd",pc->cmdBuf,3))
                    {   
                        system("clear");
                        bzero(pd,sizeof(data_t));
                        recv(socketFd,&pd->len,sizeof(int),0);
                        recvCycle(socketFd,pd->buf,pd->len);
                        printf("%s\n",pd->buf);
                    }
                }
            }   
        }else{
            continue;
        }
        return 0;
    }
}
