#ifndef __QUE_WORK_H__
#define __QUE_WORK_H__
#include "head.h"

typedef struct tag_node{
    int newFd;
    struct tag_node *pNext;
}Node_t,*pNode_t;

typedef struct{
    pNode_t queHead,queTail;
    int queCapacity;
    int queSize;
    pthread_mutex_t mutex;
}Que_t,*pQue_t;

void queInit(pQue_t,int);
void queInsert(pQue_t,pNode_t);
int queGet(pQue_t,pNode_t*);
#endif
