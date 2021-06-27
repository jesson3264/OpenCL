/*********************************
single linked queue
*********************************/

#ifndef __SQUEUE_H__
#define __SQUEUE_H__
#include "dscommon.h"

/*
@function : 
initialization
*/
void squeue_init(sq_node_t* squeue);
/*
@function :
Inserts a new element at the end of the queue, after its current last element. 
The content of this new element is initialized to val.
*/
void squeue_push(sq_node_t* squeue, sq_node_t* newnode);
/*
@function : 
Removes the next element in the queue, effectively reducing its size by one.
The element removed is the "oldest" element in the queue whose value can be retrieved by calling member queue::front.
This calls the removed element's destructor.
*/
sq_node_t* squeue_pop(sq_node_t* squeue);
/*
@function :
Returns a pointer to the last element in the queue. 
	This is the "newest" element in the queue 
	(i.e. the last element pushed into the queue).
*/
sq_node_t* squeue_back(sq_node_t* squeue);

sq_node_t* squeue_front(sq_node_t* squeue);

/*
@function :
Returns whether the queue is empty: i.e. whether its size is zero.
*/
int squeue_empty(sq_node_t* squeue);




#endif