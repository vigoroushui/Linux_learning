#include <func.h>

void sigFunc(int signum)
{
    printf("before sleep %d is coming\n",signum);
    sleep(3);
    printf("after sleep %d is coming\n",signum);
}
int main()
{
    if(SIG_ERR==signal(SIGQUIT,sigFunc))
    {
        perror("signal");
        return -1;
    }
    signal(SIGINT,sigFunc);
    while(1);
    return 0;
}

