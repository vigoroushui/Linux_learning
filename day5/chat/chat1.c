#include <func.h>
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int fdr=open(argv[1],O_RDONLY);
    int fdw=open(argv[2],O_WRONLY);
    //printf("I am chat1 fdr=%d,fdw=%d\n",fdr,fdw);
    char buf[256]={0};
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
                if(0==ret)
                {
                    printf("CHAT OVER!\n");
                    break;
                }
                write(fdw,buf,strlen(buf)-1);
            }
            if(FD_ISSET(fdr,&rdset))
            {
                memset(buf,0,sizeof(buf));
                ret=read(fdr,buf,sizeof(buf));
                if(0==ret)
                {
                    printf("CHAT OVER BY OPPSITE\n");
                    break;
                }
                printf("Vigoroushui: %s\n",buf);
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

