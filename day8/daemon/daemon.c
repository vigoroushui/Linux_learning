#include <func.h>
#define MAXTIME 1000
void Daemon()
{
    int i=0;
    if(fork()!=0)
    {
        exit(0);
    }
    setsid();
    chdir("/");
    umask(0);
    for(;i<64;i++)
    {
        close(i);
    }
}
int main(int argc,char* argv[])
{   
    ARGS_CHECK(argc,2);
    Daemon();
    int i=0;
    int fp=open(argv[1],O_WRONLY|O_CREAT);
    ERROR_CHECK(fp,-1,"open");
    while(i<MAXTIME)
    {
        write(fp,"wohenshuai",10);
        i++;
        sleep(60);
    }
    return 0;
}

