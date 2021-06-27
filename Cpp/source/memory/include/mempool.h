#ifndef __XMEMPOOL_MEMPOOL_H_
#define __XMEMPOOL_MEMPOOL_H_
#include <map>
using std::map;

typedef struct Node Node;
typedef struct BucketInfo BucketInfo;

struct BucketInfo
{
	Node *data;
	int bucketDepth;
	int surplus; /**/ 
};

struct Node
{
	Node* next;
};

class Parameter
{
public:
	static const int Align = 8;
	static const int MaxBytes = 128;
	static const int Buckets = MaxBytes / Align;/* 20 buckets, step 8 bytes*/
};

class XMempool
{
public:
	XMempool();
	~XMempool();
	void Init(size_t depths);
	void* GetMem(int size);

	 
private:
	size_t roundup(int size)
	{
		return ((size + Parameter::Align - 1) & ~(Parameter::Align - 1));
	}

	/**/
	std::map<int, BucketInfo> m_bucket;
	size_t m_bucketDepths;
};

#endif