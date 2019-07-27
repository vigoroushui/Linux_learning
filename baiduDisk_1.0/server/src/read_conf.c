#include "ftp.h"

int get_arg(char *line,char *arg)
{
    if(line[strlen(line)-1]=='\n')
    {
        line[strlen(line)-1]='\0';
    }
    char *p=strchr(line,'=');
    ERROR_CHECK(p,NULL,"strchr");
    strcpy(arg,p+1);
    return 0;
}
int read_conf(char *buf,char *ip,char *port)
{   
    FILE *fp=fopen(buf,"r");
    ERROR_CHECK(fp,NULL,"fopen");
    char line[128];
    bzero(line,sizeof(line));
    fgets(line,sizeof(line),fp);
    //printf("%s",line);
    get_arg(line,ip);
    printf("%s\n",ip);
    //有重复代码，封函数调用
    bzero(line,sizeof(line));
    fgets(line,sizeof(line),fp);
    // printf("%s",line);
    get_arg(line,port);
    printf("%s\n",port);
    return 0;
}
//int main(int argc,char* argv[])
//{
//    ARGS_CHECK(argc,2);
//    char ip[20]={0};
//    char port[10]={0};
//    read_conf(argv[1],ip,port);
//    return 0;
//}
