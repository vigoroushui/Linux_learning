#include <func.h>
#define N 20000000
int val=0;
typedef struct{
    pthread_mutex_t mutex;
}Data_t;
void *threadFunc(void *p)
{
    int i;
    Data_t *pVal=(Data_t*)p;
    for(i=0;i<N;i++)
    {
        pthread_mutex_lock(&pVal->mutex);
        val+=1;
        pthread_mutex_unlock(&pVal->mutex);
    }
    return NULL;
}
int main()
{
    pthread_t pthid;
    Data_t threadInfo;
    int ret=pthread_mutex_init(&threadInfo.mutex,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_init");
    ret=pthread_create(&pthid,NULL,threadFunc,&threadInfo);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    int i;
    for(i=0;i<N;i++)
    {
        pthread_mutex_lock(&threadInfo.mutex);
        val+=1;
        pthread_mutex_unlock(&threadInfo.mutex);
    }
    pthread_join(pthid,NULL);
    printf("I am main thread,val=%d\n",val);
    return 0;
}

