#include "kernel.h"

ListHead *tmp;

ListHead ready, block, free;
PCB idle, *current = &idle;

void
schedule(void) {
	/* implement process/thread schedule here */

	if (!list_empty(&ready)) {
		current = list_entry(current->list.next, PCB, list);
		while(& current->list== &ready || current == &idle) {
			current = list_entry(current->list.next, PCB, list);
		}

	}

}
