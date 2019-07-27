#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
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
#include <errno.h>
#include <sys/wait.h>
#include <syslog.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/uio.h>
#include <sys/sendfile.h>

#define ARGS_CHECK(argc,val) {if(argc!=val)  {printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retVal,funcName) {if(ret==retVal) {perror(funcName);return -1;}}
#define THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) {printf("%s:%s\n",funcName,strerror(ret));return -1;}}
#define CHILD_THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) {printf("%s:%s\n",funcName,strerror(ret));return (void*)-1;}
#define FILENAME "file"

typedef struct{
    pid_t pid;//child process : pid
    int pipeFd;//the opposite terminal of chile process's pipe
    short busy;//mark the status of chile process,0 represent leisure,1 represent busy
}process_data_t;

typedef struct{
    int dataLen;//Data stored in buf,which is called train head.
    char buf[1000];//train carriage
}train_t;

int makeChild(process_data_t*,int);
int childHandle(int);
int sendFd(int,int);
int recvFd(int,int*);
int tcpInit(int*,char*,char*);
int epollInAdd(int,int);
int tranFile(int);
