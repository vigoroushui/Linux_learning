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

int loginRegister(int newFd,char* username,MYSQL* conn)
{   
    train_t userName;
    bzero(&userName,sizeof(train_t));
    recv(newFd,&userName.dataLen,sizeof(int),0);
    recvCycle(newFd,userName.buf,userName.dataLen);
    strcpy(username,userName.buf);
    char query[30]="select * from userVerify";
    int flagRegister;
    mysql_query(conn,query);
    MYSQL_RES *res=mysql_use_result(conn);//将检索结果按行进行初始化操作
    MYSQL_ROW row;
    if(res)
    {
        while((row=mysql_fetch_row(res))!=NULL)//获取每一行数据大小信息
        {	
            if(0==strcmp(userName.buf,row[0]))
            {
                int flag=-1;//代表用户已存在
                send(newFd,&flag,sizeof(int),0);
                flagRegister=0;    
                break;
            }
        }
    }
    if(NULL==row||0==res)
    {   
        int flag=0;
        send(newFd,&flag,sizeof(int),0);
        char saltTmp[24]={0};//随机生成一个盐值
        getRandomSalt(saltTmp,12);
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
    mysql_free_result(res);
    mysql_close(conn);
    return flagRegister;
}

int loginVerify(int newFd,char* username,MYSQL* conn)
{   
    train_t userName;
    bzero(&userName,sizeof(train_t));
    recv(newFd,&userName.dataLen,sizeof(int),0);
    recvCycle(newFd,userName.buf,userName.dataLen);
    strcpy(username,userName.buf);
    MYSQL_ROW row;
    char query[200]="select * from userVerify";
    int flagVerify=0;
    mysql_query(conn,query);
    MYSQL_RES *res=mysql_use_result(conn);//将检索结果按行进行初始化操作
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
    mysql_close(conn);
    return flagVerify;
}

