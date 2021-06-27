#include "DataNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

namespace DS
{
	DataNode::DataNode()
	{
		_capacity = DEFAULT_BUFF_LEN;
		_buff = new char[_capacity];
		this->init();
	}

	DataNode::DataNode(int len)
	{
		if (len <= 0)
		{
			_buff = NULL;
		}

		_capacity = len;
		_buff = new char[_capacity];
		this->init();
	}

	void DataNode::init()
	{
		_from = 0;
		_to = 0;
		memset(_buff, 0, _capacity);
	}

	DataNode::~DataNode()
	{
		if (_buff)
		{
			delete[]_buff;
			_buff = NULL;
		}
	}

	//
	int DataNode::ReadData(char* dst, int len, int type)
	{
		if (!dst || (DATA_GET_TYPE_DEFAULT != type  && DATA_GET_TYPE_PEEK != type))
		{
			return -1;
		}

		int cpy_len = std::min(len, GetDataLen());
		memcpy(dst, _buff + _from, cpy_len);
		if (DATA_GET_TYPE_DEFAULT == type)
		{
			_from += cpy_len;
		}

		return cpy_len;
	}

	int DataNode::GetDataLen() const
	{
		return _to - _from;
	}

	int DataNode::GetCapacity()
	{
		return _capacity;
	}

	int DataNode::PushData(const char* src, int len)
	{
		if (len <= 0)
		{
			return -1;
		}
		// 如果左边有剩余空间，作一次 marshal，减少上层对 DataNode 的分配
		if (_from)
		{
			int data_len = this->GetDataLen();
			memmove(_buff, _buff + _from, data_len);
			_to -= _from;
			_from = 0;
		}

		int right_space_len = this->GetSpaceLen() - _to;
		int minor_len = std::min(len, right_space_len);
		memcpy(_buff + _to, src, minor_len);
		_to += minor_len;
		return minor_len;
	}


	int DataNode::GetSpaceLen()
	{
		if (_from)
		{
			int data_len = this->GetDataLen();
			memmove(_buff, _buff + _from, data_len);
			_to -= _from;
			_from = 0;
		}

		return _capacity - _to;
	}

	char* DataNode::GetBuff()
	{
		return _buff + _from;
	}

	int DataNode::AdjustLen(int diff)
	{
		if (diff == 0)
		{
			return 0;
		}

		if (diff < 0)
		{
			int dlen = this->GetDataLen();
			int minor = (dlen < abs(diff)) ? (dlen) : (abs(diff));
			_to -= minor;
			// 将右边的数据置空，方便字符串处理函数操作 
			memset(_buff + _to, 0, _capacity - _to);
			return minor;
		}
		else if (diff > 0)
		{
			int spacelen = this->GetSpaceLen();
			int minor = (spacelen < diff) ? (spacelen) : (diff);
			_to += minor;
			memset(_buff + _to, 0, _capacity - _to);
			return minor;
		}

		return 0;
	}

}// namespace DS

