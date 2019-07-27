#include "ftp.h"
//关键在于server端和client端的协议设计。
int loginVerify(int newFd,char* username)
{   
    train_t userName;
    bzero(&userName,sizeof(train_t));
    recv(newFd,&userName.dataLen,sizeof(int),0);
    recvCycle(newFd,userName.buf,userName.dataLen);
    strcpy(username,userName.buf);
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="xyh";
    char* database="baiduDisk";//要访问的数据库名称
    char query[200]="select * from userVerify";
    puts(query);
    int t,flagVerify=0;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        printf("Connected...\n");
    }
    t=mysql_query(conn,query);//mysql_real_query(),如果字符串包含二进制数据（其中可能包含NULL字节），那么你必须使用mysql_real_query()。
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //	printf("Query made...\n");
        res=mysql_use_result(conn);//将检索结果按行进行初始化操作
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)//mysql_fetch_lengths()获取每一行数据大小信息
            {	
                if(0==strcmp(userName.buf,row[0]))
                {
                    int flag=0;
                    send(newFd,&flag,sizeof(int),0);
                    char querySalt[30]={0};
                    strcpy(querySalt,row[2]);
                    puts(querySalt);
                    int saltLen=strlen(querySalt);
                    send(newFd,&saltLen,sizeof(int),0);
                    sendCycle(newFd,querySalt,saltLen);

                    char cryptogram[512]={0};
                    int cryptLen;
                    recv(newFd,&cryptLen,sizeof(int),0);//接收密文
                    recvCycle(newFd,cryptogram,cryptLen);

                    char cryptReal[512]={0};
                    strcpy(cryptReal,row[1]);
                    if(0==strcmp(cryptogram,cryptReal))
                    {   
                        printf("login successful\n");
                        char info[30]="login successful!\n";
                        int infoLen=strlen(info);
                        send(newFd,&infoLen,sizeof(int),0);
                        sendCycle(newFd,info,infoLen);
                        flagVerify=1;
                        break;
                    }else{
                        printf("login failed\n");
                        flagVerify=0;
                        break;
                    }
                }
            }
            if(NULL==row)
            { 
                int flag=-1;
                send(newFd,&flag,sizeof(flag),0);
                flagVerify=0;
            }
        }else{
            printf("This user is not exits\n");
            flagVerify=0;
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return flagVerify;
}
