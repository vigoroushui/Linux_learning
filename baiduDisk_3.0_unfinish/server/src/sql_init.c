#include "ftp.h"

void mysqlInit(MYSQL *connect,char* server,char* user,char* password,char* database)
{
    connect=mysql_init(NULL);
    if(!mysql_real_connect(connect,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(connect));
        return;
    }else{
        printf("connected...\n");
    }
    return;
}

