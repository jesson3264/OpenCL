/****
module: double linked list
*****/
#ifndef __DLIST_H__
#define __DLIST_H__
#include "dscommon.h"
#include <stdlib.h>

void dlist_init(d_node_t* head);

void dlist_add_tail(d_node_t* head, d_node_t* newnode);

void dlist_add_head(d_node_t* head, d_node_t* newnode);

// return the removed element pointer
d_node_t* dlist_remove_from_head(d_node_t* head);

d_node_t* dlist_remove_from_tail(d_node_t* head);

// get the first element from head.
d_node_t* dlist_peek_head(d_node_t* head);

// get the last element 
d_node_t* dlist_peek_tail(d_node_t* head);

int dlist_size(d_node_t* head);

#define dlist_for_each(pos, head) \
	for (pos = (head)->next; pos != head; pos = pos->next)

#define dlist_reverse_for_each(pos, head)  \
	for (pos = (head)->prev; pos != head; pos = pos->prev)

#endif 
