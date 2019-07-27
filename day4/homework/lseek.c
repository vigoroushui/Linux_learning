#include<func.h>

typedef struct student
{
	int num;
	char name[20];
	float score;
}student_t;

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);
	student_t s[3]={{1001,"zhangsan",99.2},{1002,"zhanger",39.2},{1003,"zhangsi",93.2}};
    student_t buf[3];
	int i;
	int fd=open(argv[1],O_RDWR);
	ERROR_CHECK(fd,-1,"open");
	for(i=0;i<3;i++)
    {
        int ret=write(fd,&s[i],sizeof(student_t));
        ERROR_CHECK(ret,-1,"write");
    }
	lseek(fd,0,SEEK_SET);
	for(i=0;i<3;i++)
	{
        read(fd,&buf[i],sizeof(student_t));
		printf("%d %s %5.2f\n",buf[i].num,buf[i].name,buf[i].score);
	}
	return 0;
}

