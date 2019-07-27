#include <func.h>
#define MAP_HUGE_2MB 1<<21

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2);
    int fp=open(argv[1],O_RDWR|O_CREAT,0666);
    ERROR_CHECK(fp,-1,"open");
    int ret;
    char *p;
    p=(char*)mmap(NULL,MAP_HUGE_2MB,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0);
    ERROR_CHECK(p,(char*)-1,"mmap");
    strcpy(p,"helloworld");
    sleep(20);
    ret=munmap(p,MAP_HUGE_2MB);
    ERROR_CHECK(ret,-1,"munmap");
    return 0;
}

