#include "ftp.h"

void sigHandle2(int signum)
{
    while(waitpid(-1,NULL,WNOHANG)>0);
}

void signalInit()
{
    signal(SIGINT,sigHandle1);
    signal(SIGCHLD,sigHandle2);
}

