#include <func.h>

typedef struct msgbuf{
    long mtype;
    char mtext[128];
}MST_t;
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int msgid=msgget(1001,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msgbuf msgInfo;
    msgInfo.mtype=atoi(argv[1]);
    if(!fork())
    {
        strcpy(msgInfo.mtext,argv[2]);
        int ret=msgsnd(msgid,&msgInfo,strlen(msgInfo.mtext),0);
        ERROR_CHECK(ret,-1,"msgsnd");
        exit(0);
    }else{
        sleep(3);
        bzero(&msgInfo,sizeof(msgInfo));
        int ret=msgrcv(msgid,&msgInfo,sizeof(msgInfo.mtext),msgInfo.mtype,0);
        ERROR_CHECK(ret,-1,"msgrcv");
        printf("mtype=%ld,mtext=%s\n",msgInfo.mtype,msgInfo.mtext);
        wait(NULL);
    }
    int ret=msgctl(msgid,IPC_RMID,NULL);
    ERROR_CHECK(ret,-1,"msgctl");
    printf("delete msg successful!\n");
    return 0;
}

