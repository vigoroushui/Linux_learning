#include<func.h>

void bit_change(unsigned int mode,char *p)
{
    char sign[3]={'r','w','x'};
    int i,k=2;
    for(i=8;i>=0;i--)
    {
        if((mode&1)==1)
            p[i]=sign[k];
        else
            p[i]='-';
        mode>>=1;
        k--;
        if(k<0)
            k=2;
    }
}
int ls_l(char *path)
{
   	DIR *dir;
	dir=opendir(path);
    ERROR_CHECK(dir,NULL,"opendir");
	struct dirent *p;
	struct stat buf;
	int ret;
    char bit[10]={0};
	ret=stat(path,&buf);
	ERROR_CHECK(ret,-1,"stat");
    char buff[1024]={0};
	while((p=readdir(dir)))
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))
	    {
			continue;
	    }
        char IsDir;
        if(4==p->d_type)
        {
            IsDir='d';
        }
        else
        {
            IsDir='-';
        }
        bit_change(buf.st_mode,bit);
        printf("%c",IsDir);
        printf("%s",bit);
		printf(" %ld %s %s %ld %s %s\n",buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,ctime(&buf.st_mtime),p->d_name);
		sprintf(buff,"%s%s%s",path,"/",p->d_name);
		if(4==p->d_type)
	    {
	        ls_l(buff);
		}
	}
	return 0;
}
int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,2);
	ls_l(argv[1]);
	return 0;
}
