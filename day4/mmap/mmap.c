#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);
    int fd;
	fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    printf("fd=%d\n",fd);
	char *p;
	p=(char *)mmap(NULL,5,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	ERROR_CHECK(p,(char*)-1,"mmap");
	p[5]=0;
	printf("%s\n",p);
	strcpy(p,"world");//最后会传入一个/0
    int ret= munmap(p,5);
	ERROR_CHECK(ret,-1,"munmap");
	close(fd);
    return 0;
}

