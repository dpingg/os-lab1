#include "kernel.h"

PCB *readyq, *freeq, *sleepq;
PCB PCB_pool[KERNEL_PCB_MAX];

/* from schedule.c */
extern PCB idle;


/*接下来你需要实现内核线程的创建, 即实现函数PCB* create_kthread(void *fun), 
它创建一个以fun为入口地址的线程, 初始化现场信息, 并返回其PCB. 你可以根据自己的需要添加函数的参数.*/

PCB*
create_kthread(void *fun) {
	if (list_empty(&(freeq->freel))) {
		return NULL;
	}
	PCB *new_kthread = freeq;

	/* delete the thread from free list, add it to the ready list */
	freeq = (PCB *)freeq->freel.next;
	list_del(&(new_kthread->freel));
	list_add_after(&(current->readyl), &(new_kthread->readyl));
	

	/* initialize the trap frame */
	new_kthread->tf = (TrapFrame *)(new_kthread->kstack + KSTACK_SIZE) - 1;
	TrapFrame *tf = (TrapFrame *)(new_kthread->tf);
	tf->irq = 1000;
	tf->eip = (uint32_t)fun;
	tf->cs = SELECTOR_KERNEL(SEG_KERNEL_CODE);
	tf->gs = tf->fs = tf->es = tf->ds = SELECTOR_KERNEL(SEG_KERNEL_DATA);
	tf->eflags = 0x202;  /* why */
/*	*(new_kthread->kstack+KSTACK_SIZE-sizeof(TrapFrame) )=a; */
	return new_kthread;

}

void print_ch (int ch) { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {printk("%c", ch);}
        x ++;
    }
}

void A () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {printk("a");}
        x ++;
    }
}
void B () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {printk("b");}
        x ++;
    }
}

void
init_proc() {
	readyq = &idle;
	list_init(&(idle.readyl));
	list_init(&(idle.sleepl));
	list_init(&(idle.freel));
	
	freeq = &PCB_pool[0];
	list_init(&(PCB_pool[0].readyl));
	list_init(&(PCB_pool[0].sleepl));
	list_init(&(PCB_pool[0].freel));

	int i;
	for (i=1; i < KERNEL_PCB_MAX; i++) {
		list_init(&(PCB_pool[i].readyl));
		list_init(&(PCB_pool[i].sleepl));
		list_add_after(&(PCB_pool[i-1].freel), &(PCB_pool[i].freel));
	}
	create_kthread(A);
	create_kthread(B);
/*	for(i = 0; i < 7; i ++) {
    	create_kthread(print_ch, 'a' + i);
	} */
}

