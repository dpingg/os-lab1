#include "kernel.h"

PCB idle, *current = &idle;

void
schedule(void) {
	/* implement process/thread schedule here */
	if (!list_empty(&(current->readyl))) {
		current = list_entry(current->readyl.next, PCB, readyl);
		if (current == &idle) {
			current = list_entry(current->readyl.next, PCB, readyl);
		}
	}

}
