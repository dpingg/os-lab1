#include "kernel.h"

PCB PCB_pool[KERNEL_PCB_MAX];

/* from schedule.c */
PCB *PCB_of_thread_A, *PCB_of_thread_B, *PCB_of_thread_C, *PCB_of_thread_D;

/*接下来你需要实现内核线程的创建, 即实现函数PCB* create_kthread(void *fun), 
它创建一个以fun为入口地址的线程, 初始化现场信息, 并返回其PCB. 你可以根据自己的需要添加函数的参数.*/

PCB*
create_kthread(void *fun, ListHead *put) {
	if (list_empty(&free)) {
		return NULL;
	}

	PCB *new_kthread = list_entry(free.next, PCB, list);

	/* delete the thread from free list, add it to the put list */
	list_del(free.next);
	list_add_after(put, &new_kthread->list);

    printk("create %x\n", &new_kthread->list);
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


void A () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printk("a");
            wakeup(PCB_of_thread_B);
            sleep();
        }
        x ++;
    }
}
void B () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printk("b");
            wakeup(PCB_of_thread_C);
            sleep();
        }
        x ++;
    }
}
void C () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printk("c");
            wakeup(PCB_of_thread_D);
            sleep();
        }
        x ++;
    }
}
void D () { 
    int x = 0;
    while(1) {
        if(x % 100000 == 0) {
            printk("d");
            wakeup(PCB_of_thread_A);
            sleep();
        }
        x ++;
    }
}

void sleep(void){
	ListHead *tmp;

/*    list_foreach(tmp, &current->list) {
        printk("bcurrent: %x\n", tmp);
    }*/
    tmp=&current->list;
	list_del(&current->list);

/*    list_foreach(tmp, &current->list) {
        printk("acurrent: %x\n", tmp);
    }*/
	list_add_after(&block, tmp);
	wait_intr(); 
	/*asm volatile("int $0x80"); */
}
void wakeup(PCB *p){
 /*   printk("%d\n", list_size(&ready));*/
	if(!list_exist(&ready, &p->list)) {
		if(list_exist(&block, &p->list)) {
			list_del(&p->list);
			list_add_after(&ready, &p->list);
		}
	}
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

	PCB_of_thread_A = create_kthread(A, &ready);
	PCB_of_thread_B = create_kthread(B, &block);
	PCB_of_thread_C = create_kthread(C, &block);
	PCB_of_thread_D = create_kthread(D, &block);


/*	for(i = 0; i < 7; i ++) {
    	create_kthread(print_ch, 'a' + i);
	} */
}

