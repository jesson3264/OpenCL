#include "mempool.h"

XMempool::XMempool()
{

}

XMempool::~XMempool()
{

}

void XMempool::Init(size_t depths)
{
	m_bucketDepths = depths;
	// 8 16 24 32 40 48 
	for (size_t i = Parameter::Align; i <= Parameter::MaxBytes; i += Parameter::Align)
	{
		char * mem = (char*)malloc(i * m_bucketDepths);
		BucketInfo info;
		info.data = (Node*)mem;
		info.bucketDepth = m_bucketDepths;
		info.surplus = m_bucketDepths;

		m_bucket.insert(std::pair<int, BucketInfo>(i, info));
		m_bucket[i].data = (Node*)mem;

		/* do link */
		Node* tail = (Node*)(mem + i * info.bucketDepth - i);
		tail->next = NULL;
		Node* cur = NULL;
		for (int j = info.bucketDepth - 1; j > 0; --j)
		{
			cur = (Node*)((char*)tail - i*sizeof(char));
			cur->next = tail;
			tail = cur;
		}
	}
}

void* XMempool::GetMem(int size)
{
	size_t s = roundup(size);


}
