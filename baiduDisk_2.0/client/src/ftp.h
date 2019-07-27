#ifndef __FUNC_H__
#define __FUNC_H__
#define _XOPEN_SOURCE
#define MAXSIZE 1024
#define MMAPSIZE 1024*1024
#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/mman.h>
#include<signal.h>
#include<shadow.h>
#include<errno.h>
#define ARGS_CHECK(argc,val) {if(argc!=val)  {printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retVal,funcName) {if(ret==retVal) {perror(funcName);return -1;}}
#define THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) {printf("%s:%s\n",funcName,strerror(ret));return -1;}}
#define CHILD_THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) {printf("%s:%s\n",funcName,strerror(ret));return (void*)-1;}}
typedef struct {
    int len;
    char buf[128];
}data_t,*pdata_t;

typedef struct{
    int cmdLen;
    char cmdBuf[MAXSIZE];
}cmd_t,*pcmd_t;

typedef struct{                                                                     
    int dataLen;//存储buf上要发送的数据上去
    char buf[128];//火车车厢
}train_t;

void identityConfirm(int);
void sendfile(int);
int recvCycle(int,char*,int);
int sendCycle(int,char*,int);
int mmapSendFile(int,char*);
int isFileExist(char* fileName);
int isFileExistTmp(char* fileName);
int recvFile(int socketFd);
int sendFile(int socketFd,char* fileName);
#endif
