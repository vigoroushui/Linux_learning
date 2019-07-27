#include <func.h>

void cleanup(void *p)
{
    free(p);
    printf("free successful\n");
}

void* threadFunc(void *p)
{
    p=(char*)malloc(20);
    strcpy((char*)p,"hello");
    pthread_cleanup_push(cleanup,p); 
    char buf[128]={0};
    read(STDIN_FILENO,buf,sizeof(buf));
    printf("buf=%s\n",buf);
    pthread_cleanup_pop(1);
    pthread_exit((void*)3);
}

int main()
{
    pthread_t pthid;
    int ret=pthread_create(&pthid,NULL,threadFunc,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    ret=pthread_cancel(pthid);
    THREAD_ERROR_CHECK(ret,"pthread_cancel");
    long threadRet;
    ret=pthread_join(pthid,(void**)&threadRet);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("I am main thread,threadReturn=%ld\n",threadRet);
    return 0;
}

