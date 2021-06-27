#include "scl_double_list.h"
void dlist_init(d_node_t* head)
{
	if (!head) 
		return ;
	head->next = head;
	head->prev = head;
}

void dlist_add_tail(d_node_t* head, d_node_t* newnode)
{
	if (!head || !newnode) 
		return ;

	dl_node_add(head->prev, head, newnode);
}

void dlist_add_head(d_node_t* head, d_node_t* newnode)
{
	if (!head || !newnode) 
		return ;

	dl_node_add(head, head->next, newnode);
}

d_node_t* dlist_remove_from_head(d_node_t* head)
{
	d_node_t* ret = NULL;
	if (!head)
		return NULL;
	//  empty
	if (head->next == head)
		return NULL;

	ret = head->next;
	dl_node_remove(head, head->next->next);
	
	return ret;
}

d_node_t* dlist_remove_from_tail(d_node_t* head)
{
	d_node_t* ret = NULL;
	if (!head)
		return  NULL;
	if (head->next == head)
		return NULL;

	ret = head->prev;
	dl_node_remove(head->prev->prev, head);
	return ret;
}

// get the first element from head.
d_node_t* dlist_peek_head(d_node_t* head)
{
	if (!head)
		return NULL;
	// empty list.
	if (head->next == head)
		return NULL;

	return head->next;
}

// get the last element 
d_node_t* dlist_peek_tail(d_node_t* head)
{
	if (!head)
		return NULL;
	if (head->next == head)
		return NULL;
	return head->prev;
}

int dlist_size(d_node_t* head)
{
	if (!head || (head->next == head))
		return 0;
	int cnt = 0;
	d_node_t* pos = NULL;
	dlist_for_each(pos, head)
	{
		cnt++;
	}

	return cnt;
}
