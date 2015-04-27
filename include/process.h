#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "adt/list.h"

#define KERNEL_PCB_MAX 64
#define KSTACK_SIZE 4096

typedef struct PCB {
	void *tf;
	uint8_t kstack[KSTACK_SIZE];
	ListHead list;
}PCB;

extern PCB *current;

PCB* create_kthread(void *fun, ListHead *put);
extern PCB idle;
extern ListHead ready, block, free;

void sleep(void);
void wakeup(PCB *p);


#endif
