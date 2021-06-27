/**
 * @file stl_allocater.h
 * @author Jesson
 * @brief  借鉴 STL 实现的内存分配器
 * @version 0.1
 * @date 2020-11-29
 * 
 * @copyright Copyright (c) 2020
 * 
 * 
 * 
 */
#ifndef MEMPOOL_STL_ALLOCATOR_H
#define MEMPOOL_STL_ALLOCATOR_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>




/**
 * @brief 
 * 
 * 
 */
class Allocator 
{
public:
    Allocator();
    ~Allocator();
private:
    enum {__ALIGN = 8};
    enum {__MAX_BYTES = 128};
    enum {__NFREELISTS = __MAX_BYTES/__ALIGN};

    static size_t ROUND_UP(size_t bytes) {
        return (((bytes) + __ALIGN-1) & ~(__ALIGN - 1));
	}

    union obj {
        union obj * free_list_link;
        char client_data[1];    /* The client sees this.        */
	};

    obj *  free_list[__NFREELISTS]; 

    // Returns an object of size n, and optionally adds to size n free list.
	void *refill(size_t n);
	// Allocates a chunk for nobjs of size "size".  nobjs may be reduced
	// if it is inconvenient to allocate the requested number.
	char *chunk_alloc(size_t size, int &nobjs);

	// Chunk allocation state.
	static char *start_free;
	static char *end_free;
	static size_t heap_size;
    #if 0
      /* n must be > 0      */
    static void * allocate(size_t n)
    {
        obj * __VOLATILE * my_free_list;
        obj * __RESTRICT result;

        if (n > (size_t) __MAX_BYTES) {
            return (malloc(n));
        }
	    //得到第几号链表
        my_free_list = free_list + FREELIST_INDEX(n);

        result = *my_free_list;
        if (result == 0) {
            void *r = refill(ROUND_UP(n));
            return r;
        }
	    //指向下一个元素
        *my_free_list = result -> free_list_link;
		return (result);
	};

      /* p may not be 0 */
	static void deallocate(void *p, size_t n)
	{
		obj *q = (obj *)p;
		obj * __VOLATILE * my_free_list;

		if (n > (size_t) __MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
            return;
        }
        my_free_list = free_list + FREELIST_INDEX(n);

        q -> free_list_link = *my_free_list;
        *my_free_list = q;
    }

    static void *refill(size_t n);
    static char *chunk_alloc(size_t size, int &nobjs);
    #endif 
};

#if 0
char* Allocator::chunk_alloc(size_t size, int& nobjs)
{
    char * result;
    size_t total_bytes = size * nobjs;
	
    size_t bytes_left = end_free - start_free;

    if (bytes_left >= total_bytes) {//如果空间足够，直接返回，并调整空闲池。
        result = start_free;
        start_free += total_bytes;
        return(result);
    } else if (bytes_left >= size) {//空间不是足够，但是够一个以上的size。
        nobjs = bytes_left/size;
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return(result);
    } 
	else 
	{
        // 最开始会走到这里 ， heap_size = 0, 最开始就是 2*nobjs*size
        // 后面 bytes_to_get 付给了 head_size，也就是， 下次要走到这里的话， 会分配 2*nobjs*size + (2*nobjs*size)>> 4
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
        // Try to make use of the left-over piece.
		//回收空闲池中的碎片。
        if (bytes_left > 0) {
            obj * __VOLATILE * my_free_list =
                        free_list + FREELIST_INDEX(bytes_left);

            ((obj *)start_free) -> free_list_link = *my_free_list;
            *my_free_list = (obj *)start_free;
        }
		//尝试从系统中获取所需空间。
        start_free = (char *)malloc(bytes_to_get);
        if (0 == start_free) {
            int i;
            obj * __VOLATILE * my_free_list, *p;
            // Try to make do with what we have.  That can't
            // hurt.  We do not try smaller requests, since that tends
            // to result in disaster on multi-process machines.
			//如果从系统中请求不到，这个时候从 free_list中更大的区块中获取区块。
            for (i = size; i <= __MAX_BYTES; i += __ALIGN) {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p) {
                    *my_free_list = p -> free_list_link;
                    start_free = (char *)p;
                    end_free = start_free + i;//？？？？？？？？？？
                    return(chunk_alloc(size, nobjs));
                    // Any leftover piece will eventually make it to the
                    // right free list.
                }
            }
		//如果临近的更大区块，获取不到。再次尝试malloc_alloc 的 allocate调用，这个函数会会调用用户定义的内存不足处理函数，
		//用户可能会尝试去施放一些暂时可以施放的空间。
	    end_free = 0;	// In case of exception.
            start_free = (char *)malloc(bytes_to_get);
            // This should either throw an
            // exception or remedy the situation.  Thus we assume it
            // succeeded.
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return(chunk_alloc(size, nobjs));
    }
}

void* Allocator::refill(size_t n)
{
    int nobjs = 20;
    char * chunk = chunk_alloc(n, nobjs);
    obj * __VOLATILE * my_free_list;
    obj * result;
    obj * current_obj, * next_obj;
    int i;

    if (1 == nobjs) return(chunk);
    my_free_list = free_list + FREELIST_INDEX(n);

    /* Build free list in chunk */
      result = (obj *)chunk;
      *my_free_list = next_obj = (obj *)(chunk + n);
      for (i = 1; ; i++) {
        current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + n);
        if (nobjs - 1 == i) {
            current_obj -> free_list_link = 0;//将最后一个置为空
            break;
        } else {
            current_obj -> free_list_link = next_obj;
        }
      }
    return(result);
}

    ///一个16个元素的数组


#endif
#endif