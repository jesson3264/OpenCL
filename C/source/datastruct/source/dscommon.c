#include "dscommon.h"

// single link node add.
void sl_node_add(s_node_t* prev, s_node_t* next, s_node_t* newnode)
{
	if (!prev || !next || !newnode)
		return ;
	newnode->next = next;
	prev->next = newnode;
}

// single link node remove.
void sl_node_remove(s_node_t* prev, s_node_t* next)
{
	if (!prev || !next)
		return ;
	prev->next = next;
}

// double link node add.
void dl_node_add(d_node_t* prev, d_node_t* next, d_node_t* newnode)
{
	if (!newnode || !prev || !next) 
		return ;
	next->prev = newnode;
	newnode->next = next;
	newnode->prev = prev;
	prev->next = newnode;
}

// remove node from prev to next.
void dl_node_remove(d_node_t* prev, d_node_t* next)
{
	prev->next = next;
	next->prev = prev;
}
