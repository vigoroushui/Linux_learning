#include "ftp.h"

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd;
    tcpClientInit(&socketFd,argv[1],argv[2]);
    while(1)
    {   
        //system("clear");
        int select;
        printLoginInfo(&select);
        send(socketFd,&select,sizeof(int),0);
        if(1==select)//注册
        {   
            system("clear");
            loginClientRegister(socketFd);
            continue;
        }
        else if(2==select)//登录
        {        
            system("clear");
            int flag=loginClientVerify(socketFd);
            if(0==flag)
            {   
                clientWork();
            }
            if(-1==flag)
            {
                continue;
            }
        }else{
            continue;
        }
    }
    return 0;
}
           //    pdata_t pd=(pdata_t)calloc(1,sizeof(data_t));
           //     pcmd_t pc=(pcmd_t)calloc(1,sizeof(cmd_t));
           //     int readLen;
           //     while(bzero(pc,sizeof(cmd_t)),fflush(stdin),(readLen=read(0,pc->cmdBuf,sizeof(pc->cmdBuf)))>0)
           //     {
           //         pc->cmdLen = strlen(pc->cmdBuf);
           //         send(socketFd,&pc->cmdLen,sizeof(int),0);
           //         sendCycle(socketFd,pc->cmdBuf,pc->cmdLen);
           //         if(0==strncmp("cd",pc->cmdBuf,2))
           //         {   
           //             system("clear");
           //             bzero(pd,sizeof(data_t));  
           //             recv(socketFd,&pd->len,sizeof(int),0);
           //             recvCycle(socketFd,pd->buf,pd->len);
           //             printf("%s\n",pd->buf);
           //         }
           //         if(0==strncmp("mkdir",pc->cmdBuf,5))
           //         {
           //             system("clear");
           //             int flag;
           //             recv(socketFd,&flag,sizeof(int),0);
           //             if(0==flag)
           //             {
           //                 printf("mkdir %s success!\n",pc->cmdBuf+6);
           //             }
           //             if(-1==flag)
           //             {
           //                 printf("mkdir fail!\n");
           //             }
           //         }
           //         if(0==strncmp("ls",pc->cmdBuf,2))
           //         {
           //             system("clear");
           //             while(1)
           //             {
           //                 bzero(pd,sizeof(data_t));
           //                 recv(socketFd,&pd->len,sizeof(int),0);
           //                 if(0==pd->len)
           //                 {
           //                     break;
           //                 }
           //                 recvCycle(socketFd,pd->buf,pd->len);
           //                 printf("%s",pd->buf);
           //             }
           //         }
           //         if(0==strncmp("gets",pc->cmdBuf,4))
           //         {
           //             system("clear");
           //             int flag;
           //             recv(socketFd,&flag,sizeof(int),0);
           //             if(0==flag)
           //             {
           //                 recvFile(socketFd);
           //             }
           //             if(-1==flag)
           //             {
           //                 printf("%s is not exist!\n",pc->cmdBuf+5);
           //             }
           //         } 
           //         if(0==strncmp("puts",pc->cmdBuf,4))
           //         {   
           //             system("clear");
           //             char fileName[256]={0};
           //             int flag;
           //             sscanf(pc->cmdBuf+5,"%s",fileName);
           //             if(0==isFileExist(fileName))
           //             {
           //                 struct stat s;
           //                 bzero(&s,sizeof(s));
           //                 stat(fileName,&s);
           //                 if(s.st_size<MMAPSIZE)
           //                 {
           //                     flag=0;
           //                     send(socketFd,&flag,sizeof(int),0);
           //                     sendFile(socketFd,fileName);
           //                 }else{
           //                     flag=0;
           //                     send(socketFd,&flag,sizeof(int),0);
           //                     mmapSendFile(socketFd,fileName);
           //                 }
           //             }else{
           //                 flag=1;
           //                 send(socketFd,&flag,sizeof(int),0);
           //             }           
           //         }
           //         if(0==strncmp("remove",pc->cmdBuf,6))
           //         {   
           //             system("clear");
           //             int flag;
           //             recv(socketFd,&flag,sizeof(int),0);
           //             if(0==flag)
           //             {
           //                 printf("remove successful!\n");
           //             }
           //             if(-1==flag)
           //             {
           //                 printf("target file is not exist!\n");
           //             }
           //         }
           //         if(0==strncmp("pwd",pc->cmdBuf,3))
           //         {   
           //             system("clear");
           //             bzero(pd,sizeof(data_t));
           //             recv(socketFd,&pd->len,sizeof(int),0);
           //             recvCycle(socketFd,pd->buf,pd->len);
           //             printf("%s\n",pd->buf);
           //         }
           //     }
           // }   
        //}else{
        //    continue;
        //}
 //       return 0;
 //   }
//}
