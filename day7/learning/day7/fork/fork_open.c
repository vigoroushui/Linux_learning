#include <func.h>

int main(int argc,char* argv[])
{
    pid_t pid;
    int fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    pid=fork();
    if(0==pid)
    {
        printf("I am child mypid=%d,myppid=%d\n",getpid(),getppid());
        write(fd,"niuniu",6);
    }else{
        printf("I am parent mypid=%d,mychildpid=%d\n",getpid(),pid);
        lseek(fd,5,SEEK_SET);
        printf("I am parent seek success\n");
        sleep(1);
    }
    return 0;
}

