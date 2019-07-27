#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd,fd1;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    printf("\n");
    dup2(STDERR_FILENO,100);
    fd1=dup2(fd,STDERR_FILENO);
    close(fd);
    printf("fd1=%d\n",fd1);
    printf("ERROR:you fail to open this file.\n");
    dup2(100,STDERR_FILENO);
    return 0;
}

