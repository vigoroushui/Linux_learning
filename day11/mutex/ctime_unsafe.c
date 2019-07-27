#include <func.h>

void* threadFunc(void *p)
{
    time_t now;
    time(&now);
    char *pArg=ctime(&now);
    printf("child thread,time: %s\n",pArg);
    sleep(3);
    printf("child thread,time: %s\n",pArg);
    pthread_exit(NULL);
}

int main()
{
    pthread_t pthid;
    pthread_create(&pthid,NULL,threadFunc,NULL);
    sleep(2);
    time_t now;
    time(&now);
    char *pArg=ctime(&now);
    printf("main thread,time: %s\n", pArg);
    int ret=pthread_join(pthid,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    return 0;
}

