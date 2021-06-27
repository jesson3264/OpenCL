#include "teststack.h"
#include "scl_stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
typedef struct Cookie
{
	st_node_t node;
	int weight;
}Cookie;

typedef struct CookieStack
{
	st_node_t stack;
	int cnt;
}CookieStack;

void teststack()
{
	CookieStack cstack;
	cstack.cnt = 0;
	stack_init(&(cstack.stack));

	assert(stack_empty(&(cstack.stack)));

	Cookie *c = NULL;
	st_node_t *s = NULL;
	int i = 0;
	for (i = 0; i < 10; ++i)
	{
		c = (Cookie*)malloc(sizeof(Cookie));
		c->weight = i * 2;
		stack_push(&(cstack.stack), &(c->node));
		cstack.cnt++;
	}

	i = 18;
	while (s = stack_pop(&(cstack.stack)))
	{
		c = container_of(s, Cookie, node);
		printf("weight:%d\n", c->weight);
		assert(c->weight == i);
		i -= 2;
		free(c);
	}


}