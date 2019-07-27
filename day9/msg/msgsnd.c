#include <func.h>

struct msg_buf{
    long mtype;
    char mtext[128];
}MSG_T;

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msg_buf msgInfo;
    msgInfo.mtype=atoi(argv[1]);
    strcpy(msgInfo.mtext,argv[2]);
    int ret=msgsnd(msgid,&msgInfo,strlen(msgInfo.mtext),0);
    ERROR_CHECK(ret,-1,"msgsnd");
    return 0;
}

