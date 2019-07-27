#include <func.h>

void* threadFunc(void *p)
{
    printf("I am child thread %s\n",(char*)p);
}

int main(int argc,char* argv[])
{
    pthread_t pthid;
    char *pArg=(char*)malloc(20);
    strcpy(pArg,"hello");
    int ret=pthread_create(&pthid,NULL,threadFunc,pArg);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    pthread_join(pthid,NULL);
    printf("I am main thread\n");
    return 0;
}

