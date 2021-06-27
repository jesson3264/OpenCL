# 编码、注释规范

## 编码规范
### 头文件包含宏
```
#ifndef __MODULENAME_FILENAME_H__
#define __MODULENAME_FILENAME_H__

#endif
```
比如线程池模块的threadpool.h 头文件，头部宏使用如下

```
#ifndef __THREADPOOL_THREADPOOL_H__
#define __THREADPOOL_THREADPOOL_H__

#endif
```


## 注释规范
### 文件注释
在文件的开头一行写入  
```
/** @file filename */
```
