#ifndef __PROCESS_CHECK_H__
#define __PROCESS_CHECK_H__
// processname 进程名  len 字符数组长度 pid 进程id
// 返回 1 进程存活， 返回 0 不存活， 返回 -1 命令错误
int process_check(char *processname, int len, int pid);

#endif 
