#include<func.h>

int Searchfp(char *path,char *file)
{
	DIR *dir;
	dir=opendir(path);
	ERROR_CHECK(dir,NULL,"opendir");
	struct dirent *p;
	char buf[1024]={0};
	while((p=readdir(dir)))
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))
		{
			continue;
		}
        sprintf(buf,"%s%s%s",path,"/",p->d_name);
		if(4==p->d_type)
		{
			Searchfp(buf,file);
		}
		if(8==p->d_type&&!strcmp(p->d_name,file))
		{
			getcwd(buf,1024);
			printf("%s%s%s\n",buf,"/",p->d_name);
			printf("\n");
			return 0;
		}
	}
	printf("Not find this file\n");
	closedir(dir);
	return 0;
}

int main(int argc,char *argv[])
{
	char file[20];
	ARGS_CHECK(argc,2);
	puts(argv[1]);
	scanf("%s",file);
	Searchfp(argv[1],file);
	return 0;
}
