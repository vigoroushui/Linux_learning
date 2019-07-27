#include <func.h>

void* threadFunc(void *p)
{
    printf("I am child thread %d\n",*(int*)p);
    pthread_exit((void*)2);
}

int main(int argc,char* argv[])
{
    pthread_t pthid;
    int a=1;
    int ret=pthread_create(&pthid,NULL,threadFunc,&a);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    long threadRet;
    pthread_join(pthid,(void**)&threadRet);
    printf("I am main thread,threadReturn=%ld\n",threadRet);
    return 0;
}

