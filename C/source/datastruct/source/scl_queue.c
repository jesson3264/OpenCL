#include "scl_queue.h"
#include "dscommon.h"
#include <stdlib.h>

/*
prev <-- | head |  --> next
queue tail -- queue head --- queue front
*/
/*
@function init, the squeue has one head node, link double linkedlist
*/
void squeue_init(sq_node_t* squeue)
{
	squeue->next = squeue;
	squeue->prev = squeue;
}

// add node to the tail.
void squeue_push(sq_node_t* squeue, sq_node_t* newnode)
{
	if (!squeue || !newnode)
		return ;
	dl_node_add(squeue->prev, squeue, newnode);
}

// remove node from the head.
sq_node_t* squeue_pop(sq_node_t* squeue)
{
	sq_node_t* ret = NULL;
	if (!squeue)
		return NULL;
	if (squeue->next == squeue)
		return NULL;
	ret = squeue->next;
	dl_node_remove(squeue, squeue->next->next);
	return ret;
}

// obtain the queue back ele
sq_node_t* squeue_back(sq_node_t* squeue)
{
	if (!squeue)
		return NULL;
	if (squeue->next == squeue)
		return NULL;
	return squeue->prev;
}

sq_node_t* squeue_front(sq_node_t* squeue)
{
	if (!squeue || (squeue->next == squeue))
		return NULL;
	return squeue->next;
}

/*
@function Returns whether the queue is empty: i.e. whether its size is zero.
*/
int squeue_empty(sq_node_t* squeue)
{
	return (squeue->next == squeue) ? 1 : 0;
}
