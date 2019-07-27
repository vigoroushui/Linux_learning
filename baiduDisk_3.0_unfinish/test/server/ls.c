#include "ftp.h"

int fileStat(char *path)
{
    DIR *dir=opendir(path);
    ERROR_CHECK(dir,NULL,"opendir");
    struct dirent *p;
    struct stat buf;
    int ret=stat(path,&buf);
    ERROR_CHECK(ret,-1,"stat");
    char tmp[512]={0};
    while((p=readdir(dir)))
    {
        if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))
        {
            continue;
        }
        char type;
        if(4==p->d_type)
        {
            type='d';
        }
        if(8==p->d_type)
        {
            type='-';
        }
        printf("%c %s %6ld\n",type,p->d_name,buf.st_size);
        sprintf(tmp,"%s%s%s",path,"/",p->d_name);
        if(4==p->d_type)
        {
            fileStat(tmp);
        }
    }
    return 0;
}
//int main(int argc,char *argv[])
//{
//    ARGS_CHECK(argc,2);
//    ls(argv[1]);
//    return 0;
//}

