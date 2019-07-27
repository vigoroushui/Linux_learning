#include <func.h>
typedef struct msg_buf{
    long mtype;
    char mtext[1024];
}Mst_t;
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct msg_buf msgInfo;
    msgInfo.mtype=1;
    int fdw=open(argv[1],O_WRONLY);
    int fdr=open(argv[2],O_RDONLY);
    char buf[1024]={0};
    fd_set rdset;
    int ret;
    struct timeval timeout;
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(fdr,&rdset);
        bzero(&timeout,sizeof(timeout));
        timeout.tv_sec=60;
        ret=select(fdr+1,&rdset,NULL,NULL,&timeout);
        if(ret>0)
        {
            if(FD_ISSET(STDIN_FILENO,&rdset))
            {
                memset(buf,0,sizeof(buf));
                ret=read(STDIN_FILENO,buf,sizeof(buf));
                write(fdw,buf,strlen(buf)-1);
                strcpy(msgInfo.mtext,buf);
                msgInfo.mtype=2;
                if(0==ret)
                {
                    msgInfo.mtype=3;
                }
                int re=msgsnd(msgid,&msgInfo,strlen(msgInfo.mtext),0);
                ERROR_CHECK(re,-1,"msgsnd");
                if(0==ret)
                {  
                    printf("CHAT OVER!\n");
                    break;
                }
                system("clear");
            }
            if(FD_ISSET(fdr,&rdset))
            {
                memset(buf,0,sizeof(buf));
                ret=read(fdr,buf,sizeof(buf));
                strcpy(msgInfo.mtext,buf);
                msgInfo.mtype=1;
                if(0==ret)
                {
                    msgInfo.mtype=3;
                }
                int re=msgsnd(msgid,&msgInfo,strlen(msgInfo.mtext),0);
                ERROR_CHECK(re,-1,"msgsnd");
                if(0==ret)
                {
                    printf("CHAT OVER BY OPPSITE\n");
                    break;
                }
            }
        }
        else
        {
            printf("Timeout,oppsite doesn't reply you!\n");
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}

