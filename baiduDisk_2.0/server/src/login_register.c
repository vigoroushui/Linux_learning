#include "ftp.h"

void getRandomSalt(char* salt,const int len)
{ 
    int i;
    srand(time(NULL));
    for(i=0;i<len;i++)
    { 
        if(0==i%3)
        { 
            salt[i]=rand()%26+'a';
        } 
        else if(1==i%3)
        { 
            salt[i]=rand()%26+'A';                                            
        } 
        else{
            salt[i]=rand()%26+'0';
        } 
    } 
    salt[i]='\0';
}
//此函数运用到需要修改对应的数据库信息,注册关键在于与用户端通信协议的设计。
//PS：如果开始时用户登录信息表中数据为空，该函数不会为其注册，代码修改很简单，我就没去动它。
int loginRegister(int newFd,char*username)
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
    char* password="xyh";//自己数据库的密码
    char* database="baiduDisk";//要访问的数据库名称
    char query[200]="select userName from userVerify";
    puts(query);
    int t,flagRegister;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        res=mysql_use_result(conn);//将检索结果按行进行初始化操作
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)//mysql_fetch_lengths()获取每一行数据大小信息
            {	
                if(0==strcmp(userName.buf,row[0]))
                {
                    int flag=-1;//代表用户已存在
                    send(newFd,&flag,sizeof(int),0);
                    flagRegister=0;    
                    break;
                }
            }
            if(NULL==row)
            {   
                int flag=0;
                send(newFd,&flag,sizeof(int),0);
                char saltTmp[24]={0};//随机生成一个盐值
                getRandomSalt(saltTmp,8);
                char salt[32]={0};   
                sprintf(salt,"%s%s%s","$6$",saltTmp,"$");
                int saltLen=strlen(salt);
                send(newFd,&saltLen,sizeof(int),0);
                sendCycle(newFd,salt,saltLen);

                char cryptogram[256]={0};
                int cryptLen;
                recv(newFd,&cryptLen,sizeof(int),0);//接收密文
                recvCycle(newFd,cryptogram,cryptLen);

                char insert[200]="insert into userVerify values('";
                sprintf(insert,"%s%s%s%s%s%s%s",insert,userName.buf,"','",cryptogram,"','",salt,"')");
                puts(insert);    
                mysql_query(conn,insert);
                char info[30]="register successful!\n";
                int infoLen=strlen(info);
                send(newFd,&infoLen,sizeof(int),0);
                sendCycle(newFd,info,infoLen);
                printf("register successful\n");
                flagRegister=1;
            }
        }else{
            printf("This user is not exits\n");//PS：问题在这儿~
            flagRegister=1;
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return flagRegister;
}
