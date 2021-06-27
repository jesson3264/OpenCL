#ifndef __COMMON_H__
#define __COMMON_H__
// single link node
typedef struct s_node_s
{
	struct s_node_s *next;
}s_node_t;

// double link node
typedef struct d_node_s
{
	struct d_node_s *next;
	struct d_node_s *prev;
}d_node_t;

typedef struct d_node_s  d_node_t;
typedef struct d_node_s sq_node_t;
typedef struct d_node_s dq_node_t;


// single link node add.
void sl_node_add(s_node_t* prev, s_node_t* next, s_node_t* newnode);

// single link node remove.
void sl_node_remove(s_node_t* prev, s_node_t* next);

// double link node add.
void dl_node_add(d_node_t* prev, d_node_t* next, d_node_t* newnode);

// remove node from prev to next.
void dl_node_remove(d_node_t* prev, d_node_t* next);

// link it's name
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t)&((TYPE *)0)->MEMBER)
#endif
// 
#define  container_of(ptr, type, member)  \
         (type *)( (char *)ptr - offsetof(type,member));

#endif
