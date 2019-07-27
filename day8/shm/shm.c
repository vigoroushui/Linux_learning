#include <func.h>

int main(int argc,char* argv[])
{
    int shmid;
    shmid=shmget(1000,1<<20,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    char *p;
    p=(char*)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char*)-1,"shmat");
    if(!fork())
    {
        printf("I am child,%p\n",p);
        puts(p);
        int ret;
        ret=shmctl(shmid,IPC_RMID,NULL);
        ERROR_CHECK(ret,-1,"shmctl");
        exit(0);
    }else{
        strcpy(p,"How are you");
        printf("I am parent, I put \"how are you\" into sharememory!%p\n",p);
        wait(NULL);
    }
}

