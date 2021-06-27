#include "scl_stack.h"
#include <stdlib.h>

void stack_init(st_node_t* stack)
{
	stack->next = stack;
	stack->prev = stack;
}

st_node_t* stack_top(st_node_t* stack)
{
	if (!stack || (stack->next == stack))
		return NULL;
	return stack->next;
}

st_node_t* stack_pop(st_node_t* stack)
{
	st_node_t* ret = NULL;
	if (!stack || (stack->next == stack))
		return NULL;

	ret = stack->next;
	dl_node_remove(stack, stack->next->next);
	return ret;
}

void stack_push(st_node_t* stack, st_node_t* node)
{
	if (!stack)
		return ;
	dl_node_add(stack, stack->next, node);
}

int stack_empty(st_node_t* stack)
{
	return (stack->next == stack) ? 1 : 0;
}
