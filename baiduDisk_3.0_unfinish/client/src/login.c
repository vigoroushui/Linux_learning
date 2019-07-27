#include "ftp.h"

void printLoginInfo(int *select)
{   
    printf("please select a way to operate:\n");
    printf("1.Register an account\n");
    printf("2.log in\n");
    int flag;
    scanf("%d",&flag);
    *select=flag;
}

void loginClientRegister(int socketFd)
{
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
        return;
    }
    if(0==flags)
    {   
        char salt[24]={0};
        int saltLen;
        recv(socketFd,&saltLen,sizeof(int),0);
        recvCycle(socketFd,salt,saltLen);
        printf("%s\n",salt);
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
    }
    return;
}
int loginClientVerify(int socketFd)
{
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
        printf("this user is not exist!\n");
        return -1;
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
    }
    return 0;
}
