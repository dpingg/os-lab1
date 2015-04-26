#include "kernel.h"

PCB PCB_pool[KERNEL_PCB_MAX];

/* from schedule.c */
extern PCB idle;
extern ListHead ready, block, free;

/*接下来你需要实现内核线程的创建, 即实现函数PCB* create_kthread(void *fun), 
它创建一个以fun为入口地址的线程, 初始化现场信息, 并返回其PCB. 你可以根据自己的需要添加函数的参数.*/

PCB*
create_kthread(void *fun) {
	if (list_empty(&free)) {
		return NULL;
	}

	PCB *new_kthread = list_entry(free.next, PCB, list);


	/* delete the thread from free list, add it to the ready list */
	list_del(free.next);
	list_add_after(&ready, &new_kthread->list);


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


void stackoverflow(int x) {
    if(x == 0)
        printk("%d ",x);
    if(x > 0)
        stackoverflow(x - 1);
}
void keep_stackoverflow() {
    while(1) {
        stackoverflow(16384*1000);
    }
}

void sleep(void){

}
void wakeup(PCB *p){

}

void
init_proc() {

	list_init(&ready);
	list_init(&block);
	list_init(&free);


	list_add_after(&ready, &idle.list);

	int i=0;
	for ( ; i < KERNEL_PCB_MAX; i++) {
		list_add_after(&free, &(PCB_pool[i].list));
	}

	create_kthread(A);
	create_kthread(B);


/*	for(i = 0; i < 7; i ++) {
    	create_kthread(print_ch, 'a' + i);
	} */
}

