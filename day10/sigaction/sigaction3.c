#include <func.h>
void sigFunc(int signum,siginfo_t *p,void *p1)
{
    printf("%d is coming, sender pid=%d, sender uid=%d\n",\
           signum,p->si_pid,p->si_uid);
    
}
int main()
{
    struct sigaction act;
    bzero(&act,sizeof(act));
    act.sa_flags=SA_SIGINFO|SA_NODEFER;
    act.sa_sigaction=sigFunc;
    int ret=sigaction(SIGQUIT,&act,NULL);
    ERROR_CHECK(ret,-1,"sigaction");
    while(1);
    return 0;
}

