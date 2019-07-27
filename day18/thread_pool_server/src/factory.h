#ifndef __factory_h__
#define __factory_h__
#include "head.h"
#include "que_work.h"

typedef struct{
    Que_t que;
    pthread_cond_t cond;
    pthread_t *pthid;//存储线程ID的起始地址
    int threadNum;
    short startFlag;
}Factory_t,*pFactory_t;
int factoryInit(pFactory_t,int,int);
int factoryStart(pFactory_t);
int tcpInit(int*,char*,char*);
int tranFile(int);
#endif


