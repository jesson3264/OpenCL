/*****************************
Stack
Stacks are a type of container, 
specifically designed to operate in a LIFO context (last-in first-out), 
where elements are inserted and extracted only from one end of the container.

*******************************/

#ifndef __CSTACK_H__
#define __CSTACK_H__
#include "dscommon.h"
// the name s_entry_t has defined, so, use st_node_t representive of stack node.

typedef struct d_node_s  st_node_t;
#define T st_node_t  
#define StackNode T

void stack_init(T* stack);

StackNode* stack_top(T* stack);

StackNode* stack_pop(T* stack);

void stack_push(st_node_t* stack, st_node_t* node);

int stack_empty(st_node_t* stack);

#undef StackNode
#undef T
#endif 
