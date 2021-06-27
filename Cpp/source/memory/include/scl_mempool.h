#ifndef MEMORY_SCL_MEMPOOL_H
#define MEMORY_SCL_MEMPOOL_H

#pragma pack(1)
typedef struct ChunkHead
{
    char flag;  ///< 标记内存是否被访问
    int size;   ///< 用户数据占用大小
    char data[0];
}ChunkHead;
#pragma pack()



#endif