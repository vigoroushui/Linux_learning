#include <func.h>
typedef struct msg_buf{
    long mtype;
    char mtext[1024];
}Mst_t;

int main(int argc,char* argv[])
{
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msg_buf msgInfo;
    while(1){
        bzero(&msgInfo,sizeof(msgInfo));
        int ret=msgrcv(msgid,&msgInfo,sizeof(msgInfo.mtext),0,0);
        ERROR_CHECK(ret,-1,"msgrcv");
        if(1==msgInfo.mtype)
        {
            printf("Oliver Queen: %s\n",msgInfo.mtext);
        }
        else if(2==msgInfo.mtype)
        {
            printf("%60s",msgInfo.mtext);
        }
        else if(3==msgInfo.mtype)
        {
            break;
        }
    }
    exit(0);
}
