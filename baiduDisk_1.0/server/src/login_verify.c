#include "ftp.h"

void getSalt(char* salt,char* passwd)
{
    int i,j;
    for(i=0,j=0;passwd[i]&&j!=3;i++)
    {   
        if('$'==passwd[i])
        {   
            j++;
        }   
        strncpy(salt,passwd,i-1);
    }   
}

int loginVerify(int newFd)
{
    train_t userVer,pwdVer;
    bzero(&userVer,sizeof(userVer));
    bzero(&pwdVer,sizeof(pwdVer));
    recv(newFd,&userVer.dataLen,sizeof(int),0);
    recvCycle(newFd,userVer.buf,userVer.dataLen);
    recv(newFd,&pwdVer.dataLen,sizeof(int),0);
    recvCycle(newFd,pwdVer.buf,pwdVer.dataLen);
    struct spwd *sp;
    if(NULL==(sp=getspnam(userVer.buf)))
    {   
        return -1;
    }
    char salt[512]={0};
    getSalt(salt,sp->sp_pwdp);
    if(strcmp(sp->sp_pwdp,crypt(pwdVer.buf,salt))!=0)
    {
        return -1;
    }else{
        return 0;
    }
}
