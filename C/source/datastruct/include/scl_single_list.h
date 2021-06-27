/***
module: singly linked list
*/
#ifndef __SLIST_H__
#define __SLIST_H__
#include "dscommon.h"

//signly link list
typedef struct slist_node_s
{
	s_node_t* head;
	s_node_t* tail;
}slist_node_t;

void slist_init(slist_node_t* slist);

void slist_add_tail(slist_node_t* slist, s_node_t* newnode);

void slist_add_head(slist_node_t* slist, s_node_t* newnode);

// remove the first element from head.
s_node_t* slist_remove_head(slist_node_t* slist);

// get the first element.
s_node_t* slist_peek_head(slist_node_t* slist);

#define slist_for_each(pos, slist) \
	for (pos = (slist)->head;  pos != NULL; pos = pos->next)

#endif 
