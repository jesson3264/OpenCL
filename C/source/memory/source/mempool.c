#include "mempool.h"
#include <stdlib.h>
#include <stdio.h>

#define scl_mymalloc malloc
#define acl_myfree  free
#define acl_msg_warn printf

typedef struct SCL_DBUF {
	struct SCL_DBUF *next;
	short  used;
	short  keep;
	size_t size;
	char  *addr;
	char   buf[1];//这只是个编程技巧，表示实际用于分配给用户的内存开始位置
} SCL_DBUF;

typedef struct SCL_DBUF_POOL{
	size_t block_size;
	size_t off;
	size_t huge;
	SCL_DBUF *head;
	char  buf[1];
}SCL_DBUF_POOL;

SCL_DBUF_POOL *scl_dbuf_pool_create(size_t block_size)
{
	SCL_DBUF_POOL *pool;
	size_t size;
	int    page_size;

#ifdef ACL_UNIX
	page_size = getpagesize();
#elif defined(ACL_WINDOWS)
	SYSTEM_INFO info;

	memset(&info, 0, sizeof(SYSTEM_INFO));
	GetSystemInfo(&info);
	page_size = info.dwPageSize;
	if (page_size <= 0)
		page_size = 4096;
#else
	page_size = 4096;
#endif

	size = (block_size / (size_t)page_size) * (size_t)page_size;
	if (size < (size_t)page_size)
		size = page_size;

	/* xxx: 为了尽量保证在调用 acl_mymalloc 分配内存时为内存页的整数倍，
	* 需要减去 sizeof(SCL_DBUF) 和 16 字节，其中 16 字节是 acl_mymalloc
	* 内部给每个内存块额外添加的控制头，在 scl_mymalloc 内部 16 字节为：
	* offsetof(MBLOCK, u.payload[0])
	*/
	size -= 16 + sizeof(SCL_DBUF);

#ifdef	USE_VALLOC
	pool = (ACL_DBUF_POOL*)valloc(sizeof(struct ACL_DBUF_POOL)
		+ sizeof(ACL_DBUF) + size);
#else
	pool = (SCL_DBUF_POOL*)scl_mymalloc(sizeof(SCL_DBUF_POOL)
		+ sizeof(SCL_DBUF) + size);
#endif

	pool->block_size = size;
	pool->off = 0;
	pool->huge = 0;
	pool->head = (SCL_DBUF*)pool->buf;
	pool->head->next = NULL;
	pool->head->keep = 1;
	pool->head->used = 0;
	pool->head->size = size;
	pool->head->addr = pool->head->buf;

	return pool;
}

void scl_dbuf_pool_destroy(SCL_DBUF_POOL *pool)
{
	SCL_DBUF *iter = pool->head, *tmp;

	while (iter) {
		tmp = iter;
		iter = iter->next;
		if ((char*)tmp == pool->buf)
			break;
		if (tmp->size > pool->block_size)
			pool->huge--;
#ifdef	USE_VALLOC
		free(tmp);
#else
		acl_myfree(tmp);
#endif
	}

#ifdef	USE_VALLOC
	free(pool);
#else
	acl_myfree(pool);
#endif
}

int scl_dbuf_pool_free(SCL_DBUF_POOL *pool, const void *addr)
{
	const char *ptr = (const char*)addr;
	SCL_DBUF *iter = pool->head, *prev = iter;

	while (iter) {
		if (ptr < iter->addr && ptr >= iter->buf) {
			iter->used--;
			break;
		}

		prev = iter;
		iter = iter->next;
	}

	if (iter == NULL) {
		acl_msg_warn("warning: %s(%d), not found addr: %p",
			__FUNCTION__, __LINE__, addr);
		return -1;
	}

	if (iter->used < 0) {
		acl_msg_warn("warning: %s(%d), used(%d) < 0",
			__FUNCTION__, __LINE__, iter->used);
		return -1;
	}

	/*说明这个buf还被分割了其他内存*/
	if (iter->used > 0 || iter->keep)
		return 0;

	/* should free the ACL_DBUF block */

	if (iter == pool->head)
		pool->head = iter->next;
	else
		prev->next = iter->next;

	pool->off -= iter->addr - iter->buf;

	if (iter->size > pool->block_size)
		pool->huge--;

	acl_myfree(iter);

	return 1;
}


static SCL_DBUF *scl_dbuf_alloc(SCL_DBUF_POOL *pool, size_t length)
{
#ifdef	USE_VALLOC
	ACL_DBUF *dbuf = (ACL_DBUF*)valloc(sizeof(ACL_DBUF) + length);
#else
	SCL_DBUF *dbuf = (SCL_DBUF*)scl_mymalloc(sizeof(SCL_DBUF) + length);
#endif
	dbuf->addr = (char*)dbuf->buf;

	dbuf->next = pool->head;
	dbuf->used = 0;
	dbuf->keep = 0;
	dbuf->size = length;
	dbuf->addr = dbuf->buf;

	pool->head = dbuf;
	if (length > pool->block_size)
		pool->huge++;

	return dbuf;
}

void *scl_dbuf_pool_alloc(SCL_DBUF_POOL *pool, size_t length)
{
	void *ptr;
	SCL_DBUF *dbuf;

	length += 4 - length % 4;
	/* 找一块可用的*/
	//block_size 初始分配的一大块
	if (length > pool->block_size)
		dbuf = scl_dbuf_alloc(pool, length);
	else if (pool->head == NULL)
		dbuf = scl_dbuf_alloc(pool, pool->block_size);
	else if (pool->block_size < ((char*)pool->head->addr
		- (char*)pool->head->buf) + length)/// 剩余空间不够
	{
		dbuf = scl_dbuf_alloc(pool, pool->block_size);
	}
	else
		dbuf = pool->head;//指向可分配的头部

	ptr = dbuf->addr;
	dbuf->addr = (char*)dbuf->addr + length;//相当于头部的那个节点空间指向了下一个点
	pool->off += length;
	dbuf->used++;

	return ptr;
}
