#include <func.h>
long times=0;
void sigFunc(int signum)
{
    time_t now;
    time(&now);
    printf("%s %ld\n",ctime(&now),times);
    times=0;
}
int main()
{
    signal(SIGVTALRM,sigFunc);
    printf("before sleep\n");
    sleep(5);
    printf("after sleep\n");
    struct itimerval t;
    bzero(&t,sizeof(t));
    t.it_value.tv_sec=2;
    t.it_interval.tv_sec=1;
    sigFunc(0);
    int ret=setitimer(ITIMER_VIRTUAL,&t,NULL);
    ERROR_CHECK(ret,-1,"setitimer");
    double a,b;
    while(1)
    {
        a=12345.45;
        b=32141.21*a;
        times++;
    }
    return 0;
}

