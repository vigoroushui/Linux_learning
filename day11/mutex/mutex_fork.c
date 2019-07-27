#include <func.h>
#define N 20000000
typedef struct{
    int val;
    pthread_mutex_t mutex;
}Data_t;

int main()
{   
    int shmid=shmget(1100,1<<20,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    Data_t *p;
    p=(Data_t*)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(Data_t*)-1,"shmat");
    p->val=0;
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr,PTHREAD_PROCESS_SHARED);
    int ret=pthread_mutex_init(&p->mutex,&mattr);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_init");
    if(!fork())
    {
        int i; 
        for(i=0;i<N;i++)
        {
            pthread_mutex_lock(&p->mutex);
            p->val+=1;
            pthread_mutex_unlock(&p->mutex);
        }
        exit(0);
    }else{
        int i;
        for(i=0;i<N;i++)
        {
            pthread_mutex_lock(&p->mutex);
            p->val+=1;
            pthread_mutex_unlock(&p->mutex);
        }
        wait(NULL);
        printf("I am father process,val=%d\n",p->val);
    }
    return 0;
}

