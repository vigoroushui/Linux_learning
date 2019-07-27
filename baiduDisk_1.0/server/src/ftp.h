#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <strings.h>
#include <syslog.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/uio.h>
#include <sys/sendfile.h>
#include <shadow.h>
#define ARGS_CHECK(argc,val) {if(argc!=val)  {printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retVal,funcName) {if(ret==retVal) {perror(funcName);return -1;}}
#define THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) {printf("%s:%s\n",funcName,strerror(ret));return -1;}}
#define CHILD_THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) {printf("%s:%s\n",funcName,strerror(ret));return (void*)-1;}}
#define PROCESSNUM 5
#define MAXSIZE 1024
#define KEY 1234
#define MMAPSIZE 1024*1024
typedef struct{
    pid_t pid;//子进程的pid
    int pipeFd;//子进程的管道对端
    short busy;//用来标识子进程是否忙碌，0代表非忙碌，1代表忙碌
}process_data_t;

typedef struct{
    int cmdLen;
    char cmdBuf[MAXSIZE];
}cmd_t,*pcmd_t;//命令功能的结构体

typedef struct{
    int dataLen;//存储buf上要发送的数据上去
    char buf[128];//火车车厢
}train_t;

int get_arg(char*,char*);
int read_conf(char*,char*,char*);

int makeChild(process_data_t*,int,int);
int childHandle(int,int);
int sendFd(int,int);
int recvFd(int,int*);
int tcpInit(int*,char*,char*);
int epollInAdd(int,int);

int isFileExistTmp(char*);
int isFileExist(char*);
int recvFile(int);
int sendFile(int,char*);
int mmapSendFile(int,char*);

int recvCycle(int,char*,int);
int sendCycle(int,char*,int);
int logWrite(int,char*);
void logOption(char*,int,key_t,struct sembuf*,char*,struct sockaddr_in);
void doCd(int,pcmd_t);
void doMkdir(int,pcmd_t);
int doLs(int);
int doPuts(int,pcmd_t);
int doGets(int,pcmd_t);
int doRemove(int,pcmd_t);
void doPwd(int);
int childWork(int,int,struct sockaddr_in);

void getSalt(char*,char*);
int loginVerify(int);

void signalInit();
void sigHandle2(int);
void sigHandle1(int);
