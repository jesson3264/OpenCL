#include "stl_allocater.h"
Allocator::Allocator()
{
    for (auto i = 0 ;i < __NFREELISTS; ++i)
        free_list[i] = NULL;
}

Allocator::~Allocator()
{


}

