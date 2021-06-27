#include "scl_deque.h"
#include "scl_double_list.h"
#include <stdlib.h>

void dqueue_init(dq_node_t* dqueue)
{
	if (!dqueue)
		return;
	dqueue->next = dqueue;
	dqueue->prev = dqueue;
}

void dqueue_clear(dq_node_t* dqueue)
{

}

void dqueue_push_back(dq_node_t* dqueue, dq_node_t* newnode)
{
	if (!dqueue || !newnode)
		return;
	dl_node_add(dqueue->prev, dqueue, newnode);
}

void dqueue_push_front(dq_node_t* dqueue, dq_node_t* newnode)
{
	if (!dqueue || !newnode)
		return;
	dl_node_add(dqueue, dqueue->next, newnode);
}

dq_node_t* dqueue_pop_back(dq_node_t* dqueue)
{
	return dlist_remove_from_tail(dqueue);
}

dq_node_t* dqueue_pop_front(dq_node_t* dqueue)
{
	dq_node_t* ret = NULL;
	if (!dqueue)
		return NULL;
	if (dqueue->next == dqueue)
		return NULL;
	ret = dqueue->next;
	dl_node_remove(dqueue, dqueue->next->next);
	return ret;
}

dq_node_t* dqueue_front(dq_node_t* dqueue)
{
	return dlist_peek_head(dqueue);
}

dq_node_t* dqueue_back(dq_node_t* dqueue)
{
	return dlist_peek_tail(dqueue);
}
