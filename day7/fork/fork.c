#include <func.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    pid=fork();
    if(0==pid)
    {
        printf("I am child process,pid=%d,ppid=%d\n",getpid(),getppid());
        while(1);
    }
    else
    {
        printf("I am parent process,pid=%d,childid=%d\n",getpid(),pid);
        while(1);
    }
    return 0;
}

