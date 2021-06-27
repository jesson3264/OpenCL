
#ifndef __BUFFER_NODE_H__
#define __BUFFER_NODE_H__
/*
 * DataNode 说明：
 * [from,to] 为实际有用的数据，其他为空闲空间，
 * 压入数据在后面压入,取数据从前面开始取，
 * 取数据可以决定是否保留原数据, 类似 recv 函数的 MSG_PEEK 选项
 *
 */
// buff [..............................]
//			|有效数据区    |
//			_from		   _to

#define DATA_GET_TYPE_DEFAULT 0
#define DATA_GET_TYPE_PEEK 1
#define DEFAULT_BUFF_LEN 4096
namespace DS
{

class DataNode
{
public:
	DataNode();
	DataNode(int len);
	~DataNode();	
	// 返回实际获取到的字节数
	int ReadData(char* dst, int len, int type = DATA_GET_TYPE_DEFAULT);
	// buff 容量
	int GetCapacity();
	// 获取实际数据长度 to-from
	int GetDataLen() const;
	// 获取右部空闲长度
	int GetSpaceLen();
	// 返回实际压入的字节数
	int PushData(const char* src, int len);
	
	// 返回 buff 起始地址
	char* GetBuff(); 
	// 增加/减少数据的长度 , 返回实际调整的长度 
	int AdjustLen(int diff);
private:
	void init();
private:
	char* _buff;
	int _capacity;
	int	_from;
	int _to;
};

}// namespace DS

#endif 
