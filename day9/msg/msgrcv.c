#include <func.h>

struct msg_buf{
    long mtype;
    char mtext[128];
}MSG_T;

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2);
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msg_buf msgInfo;
    msgInfo.mtype=atoi(argv[1]);
    bzero(&msgInfo,sizeof(msgInfo));
    int ret=msgrcv(msgid,&msgInfo,sizeof(msgInfo.mtext),msgInfo.mtype,0);
    ERROR_CHECK(ret,-1,"msgrcv");
    printf("mtype=%ld,mtext=%s\n",msgInfo.mtype,msgInfo.mtext);
    return 0;
}

