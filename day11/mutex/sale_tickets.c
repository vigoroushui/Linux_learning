#include <func.h>
#define N 10
typedef struct{
    int tickets;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}Data_t;

void* saleWindows1(void *p)
{
    Data_t* pArg=(Data_t*)p;
    int i=0;
    while(1)
    {
        pthread_mutex_lock(&pArg->mutex);
        if(pArg->tickets>0)
        {
            printf("saleWindows1 start sale:%d\n",pArg->tickets);
            pArg->tickets--;
            i++;
            if(!pArg->tickets)
            {
                pthread_cond_signal(&pArg->cond);
            }
            printf("saleWindows1 finish sale:%d\n",pArg->tickets);
            pthread_mutex_unlock(&pArg->mutex);
            sleep(1);
        }else{
            pthread_mutex_unlock(&pArg->mutex);
            printf("I am saleWindows1,%d\n",i);
            break;
        }
    }
    return NULL;
}

void* saleWindows2(void *p)
{
    Data_t* pArg=(Data_t*)p;
    int i=0;
    while(1)
    {
        pthread_mutex_lock(&pArg->mutex);
        if(pArg->tickets>0)
        {
            printf("saleWindows2 start sale:%d\n",pArg->tickets);
            pArg->tickets--;
            i++;
            if(!pArg->tickets)
            {
                pthread_cond_signal(&pArg->cond);
            }
            printf("saleWindows2 finish sale:%d\n",pArg->tickets);
            pthread_mutex_unlock(&pArg->mutex);
            sleep(1);
        }else{
            pthread_mutex_unlock(&pArg->mutex);
            printf("I am saleWindows2,%d\n",i);
            break;
        }
    }
    return NULL;
}

void* resetTickets(void *p)
{
    Data_t* pArg=(Data_t*)p;
    pthread_mutex_lock(&pArg->mutex);
    if(pArg->tickets>0)
    {
        pthread_cond_wait(&pArg->cond,&pArg->mutex);
    }
    pArg->tickets=N;
    pthread_mutex_unlock(&pArg->mutex);
}

int main()
{
    Data_t threadInfo;
    threadInfo.tickets=N;
    pthread_mutex_init(&threadInfo.mutex,NULL);
    pthread_cond_init(&threadInfo.cond,NULL);
    pthread_t pthid1,pthid2,pthid3;
    pthread_create(&pthid1,NULL,saleWindows1,&threadInfo);
    pthread_create(&pthid2,NULL,saleWindows2,&threadInfo);
    pthread_create(&pthid3,NULL,resetTickets,&threadInfo);
    int ret;
    long threadRet;
    ret=pthread_join(pthid2,(void**)&threadRet);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    ret=pthread_join(pthid1,(void**)&threadRet);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    ret=pthread_join(pthid3,(void**)&threadRet);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("sale over\n");
    return 0;
}

