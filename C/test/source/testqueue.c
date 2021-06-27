#include "testqueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
typedef struct Customer
{
	sq_node_t node;
	int age;
}Customer;

typedef struct CustomerQueue
{
	sq_node_t squeue;
	int cnt;
}CustomerQueue;

void testqueue()
{
	CustomerQueue  squeue;
	squeue.cnt = 0;
	squeue_init(&(squeue.squeue));

	assert(squeue_empty(&squeue.squeue));
	int i = 0;
	Customer* c = NULL;
	for (i = 0; i < 4; ++i)
	{
		c = (Customer*)malloc(sizeof(Customer));
		c->age = i;
		squeue_push(&squeue.squeue, &(c->node));
		squeue.cnt++;
	}

	assert(squeue.cnt == 4);

	Customer *p = NULL;
	i = 0;
	sq_node_t *node;
	while (node = squeue_pop(&squeue.squeue))
	{
		p = container_of(node, Customer, node);
		printf("age:%d\n", p->age);
		assert(p->age == i);
		i++;
		free(p);
	}

	assert(squeue_empty(&squeue.squeue));
}

typedef struct SuperCustomer
{
	dq_node_t node;
	int age;
}SuperCustomer;

void testdqueue()
{
	SuperCustomer dqueue;
	dqueue_init(&dqueue.node);

	SuperCustomer* c = NULL;
	int i = 0;
	for (i = 0; i < 4; ++i)
	{
		c = (SuperCustomer*)malloc(sizeof(SuperCustomer));
		c->age = i;
		dqueue_push_back(&dqueue.node, &(c->node));
	}

	SuperCustomer *p = NULL;
	i = 0;
	dq_node_t *node;
	while (node = dqueue_pop_front(&dqueue.node))
	{
		p = container_of(node, SuperCustomer, node);
		printf("age:%d\n", p->age);
		assert(p->age == i);
		i++;
		free(p);
	}


}


void testdqueue2()
{
	SuperCustomer dqueue;
	dqueue_init(&dqueue.node);

	SuperCustomer* c = NULL;
	int i = 0;
	for (i = 0; i < 4; ++i)
	{
		c = (SuperCustomer*)malloc(sizeof(SuperCustomer));
		c->age = i;
		dqueue_push_back(&dqueue.node, &(c->node));
	}

	SuperCustomer *p = NULL;
	i = 3;
	dq_node_t *node;
	while (node = dqueue_pop_back(&dqueue.node))
	{
		p = container_of(node, SuperCustomer, node);
		printf("age:%d\n", p->age);
		assert(p->age == i);
		i--;
		free(p);
	}
}