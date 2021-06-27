#include "scl_single_list.h"
#include <stdlib.h>

void slist_init(slist_node_t* slist)
{
	if (!slist)	
		return ;
	slist->head = NULL;
	slist->tail = NULL;
}

void slist_add_tail(slist_node_t* slist, s_node_t* newnode)
{
	if (!slist || !newnode)
		return;
	newnode->next = NULL;
	if (!slist->head)
	{
		slist->head = newnode;
		slist->tail = newnode;
	}
	else
	{
		slist->tail->next = newnode;
		slist->tail = newnode;
	}
}

void slist_add_head(slist_node_t* slist, s_node_t* newnode)
{
	if (!slist || !newnode)
		return ;

	newnode->next = slist->head;
	if (!slist->tail)
	{
		slist->tail = newnode;
	}

	slist->head = newnode;
}


// remove the first element from head.
s_node_t* slist_remove_head(slist_node_t* slist)
{
	s_node_t *ret = NULL;
	if (!slist || !slist->head)
		return NULL;
	
	ret = slist->head;
	if (slist->head == slist->tail)
		slist->head = slist->tail = NULL;
	else
		slist->head = slist->head->next;
	
	return ret;
}

// get the first element
s_node_t* slist_peek_head(slist_node_t* slist)
{
	if (!slist) 
		return NULL;
	
	return slist->head;
}


