#include <func.h>
struct protocol{
    int flag;
    char buf[1024];
}prtocol_t;

int main(int argc,char* argv[])
{
    int shmid;
    shmid=shmget(1000,1<<20,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    struct protocol *p;
    p=(struct protocol *)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(struct protocol*)-1,"shmat");
    char buf[1024]={0};
    while(1)
    {
        if(2==p->flag)
        {
            strcpy(buf,p->buf);
            printf("%60s",buf);
            p->flag=0;
        }
        if(1==p->flag)
        {
            strcpy(buf,p->buf);
            printf("Vigoroushui: %s\n",buf);
            p->flag=0;
        }
        if(3==p->flag)
        {
            break;
        }
    }
    exit(0);
}

