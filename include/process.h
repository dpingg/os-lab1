#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "adt/list.h"

#define KERNEL_PCB_MAX 64
#define KSTACK_SIZE 4096

typedef union PCB {
	uint8_t kstack[KSTACK_SIZE];
	struct {
		void *tf;
		ListHead readyl, sleepl, freel;
	};
}PCB;
extern PCB *current;

PCB* create_kthread(void *fun);


#endif
