#ifndef __LIST_H__
#define __LIST_H__

#include "assert.h"

struct ListHead {
	struct ListHead *prev, *next;
};
typedef struct ListHead ListHead;

/* list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	((type*)((char*)(ptr) - (int)(&((type*)0)->member)))

static inline void
list_add(ListHead *prev, ListHead *next, ListHead *data) {
	assert(data != NULL);
	data->prev = prev;
	data->next = next;
	if (prev != NULL) prev->next = data;
	if (next != NULL) next->prev = data;
}

static inline void
list_add_before(ListHead *list, ListHead *data) {
	assert(list != NULL);
	list_add(list->prev, list, data);
}

static inline void
list_add_after(ListHead *list, ListHead *data) {
	assert(list != NULL);
	list_add(list, list->next, data);
}

static inline void
list_del(ListHead *data) {
	assert(data != NULL);
	ListHead *prev = data->prev;
	ListHead *next = data->next;
	if (prev != NULL) prev->next = next;
	if (next != NULL) next->prev = prev;
}

static inline void
list_init(ListHead *list) {
	assert(list != NULL);
	list->prev = list->next = list;
}

static inline bool
list_empty(ListHead *list) {
	assert(list != NULL);
	return list == list->next;
}



#define list_foreach(ptr, head) \
	for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)


static inline uint32_t
list_size(ListHead *list) {
	uint32_t size=0;
	ListHead *tmp;
	list_foreach(tmp, list) {
		size++;
	}
	return size;
}
#endif

