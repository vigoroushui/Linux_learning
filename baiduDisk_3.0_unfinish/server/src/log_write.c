#include "func.h"

int logWrite(int logFd,char *logBuf)
{   
    time_t currentTime;
    time(&currentTime);
    char *opTime=ctime(&currentTime);
    char divide[80]="----------------------------------------------------\n";
    write(logFd,opTime,strlen(opTime));
    write(logFd,logBuf,strlen(logBuf));
    write(logFd,divide,strlen(divide));
    return 0;
}

void logOption(char* logBuf,int logFd,key_t semid,struct sembuf *sop,char* info,struct sockaddr_in client)
{
    //memset(&logBuf,0,sizeof(logBuf));
    bzero(logBuf,sizeof(char));
    sprintf(logBuf,"client IP=%s, port=%d\n%s",inet_ntoa(client.sin_addr),ntohs(client.sin_port),info);
    semop(semid,(struct sembuf*)&sop[1],1);
    logWrite(logFd,logBuf);
    semop(semid,(struct sembuf*)&sop[0],1);
}
