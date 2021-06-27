#ifndef __DQUEUE_H__
#define __DQUEUE_H__
#include "dscommon.h"

//
void dqueue_init(dq_node_t* dqueue);

void dqueue_clear(dq_node_t* dqueue);

void dqueue_push_back(dq_node_t* dqueue, dq_node_t* newnode);

void dqueue_push_front(dq_node_t* dqueue, dq_node_t* newnode);

dq_node_t* dqueue_pop_back(dq_node_t* dqueue);

dq_node_t* dqueue_pop_front(dq_node_t* dqueue);

dq_node_t* dqueue_front(dq_node_t* dqueue);

dq_node_t* dqueue_back(dq_node_t* dqueue);



#endif