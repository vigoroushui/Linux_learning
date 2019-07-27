#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd,fd1;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    printf("\n");
    dup2(STDOUT_FILENO,100);
    fd1=dup2(fd,STDOUT_FILENO);
    close(fd);
    printf("stdout,new helloworld!\n");
    dup2(100,STDOUT_FILENO);
    printf("Not stdout,so you can see me!\n");
    return 0;
}

