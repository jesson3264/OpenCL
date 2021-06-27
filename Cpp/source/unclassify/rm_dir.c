#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

int remove_dir_sub(char *dir_full_path);
int mylog(const char *format, ...)
{

	return 0;
}

// 如果要将LOG 信息显示到自己制定目录，更改   printf
#ifdef HASE_DEBUF_INFO
	#define LOG  printf
#else //
	#define LOG  mylog
#endif 

/*************************************************
rmdir 函数只能删除空目录。
1. 递归删除目录,自己也删除
**************************************************/

int remove_dir_sub_and_self(char *dir_full_path)
{    
	int ret = remove_dir_sub(dir_full_path);
	if (ret != 0)
	{
		LOG("remove_dir_sub failured. errno:%d\n", errno);
		return -1;
	}
	
	ret = rmdir(dir_full_path);
	LOG("ret:%d\n", ret);
    return ret;
}
/*************************************************
0: 返回成功 其他值，出现错误
1. 只删除子目录和子文件
*****************************************************/
int remove_dir_sub(char *dir_full_path)
{
	if (!dir_full_path)
	{
		return -1;
	}

    DIR* dirp = opendir(dir_full_path);    
    if (!dirp)
    {
        return -1;
    }
    struct dirent *dir;
    struct stat st;
    while((dir = readdir(dirp)) != NULL)
    {
        if(strcmp(dir->d_name,".") == 0
                || strcmp(dir->d_name,"..") == 0)
        {
            continue;
        }    

        char sub_path[520] = {0};
        if (sizeof(sub_path) - 1 <= strlen(dir_full_path) + 1 + strlen(dir->d_name))	
		{
			break;
		}

        strncpy(sub_path, dir_full_path, strlen(dir_full_path));
        strcat(sub_path, "/");
        strcat(sub_path, dir->d_name);

        //std::string sub_path = dir_full_path + '/' + dir->d_name;

        if(lstat(sub_path,&st) == -1)
        {
            //Log("rm_dir:lstat ",sub_path," error");
            continue;
        }    

        if(S_ISDIR(st.st_mode))
        {
            if(remove_dir_sub(sub_path) == -1) // 如果是目录文件，递归删除
            {
                closedir(dirp);
                return -1;
            }
            rmdir(sub_path);
        }
        else if(S_ISREG(st.st_mode))
        {
            unlink(sub_path);     // 如果是普通文件，则unlink
        }
        else
        {
            //printf("rm_dir:st_mode ",sub_path," error");
            continue;
        }
    }

    closedir(dirp);
	return 0;
}

void test_remove_sub()
{
	remove_dir_sub("./testdir");	
}

void test_del_self()
{
	remove_dir_sub_and_self("./testdir");
}


int test_main(int argc, char *argv[])
{

	if (argc >= 2)
	{
		if (strcmp(argv[1], "1") == 0)
		{
			test_remove_sub();
		}
		else if (strcmp(argv[1], "2") == 0)
		{
			test_del_self();
		}
	}

#if 0
	time_t nTime = time(NULL);
	struct stat rStat;
	struct dirent *pDirent = NULL;

	char chFilePathName[260] = {0};

	DIR *pDir = opendir("./ProxyLog/");
	if (!pDir) return 0;

	while ((pDirent = readdir(pDir))!=NULL)
	{
		if (strcmp(pDirent->d_name, ".") == 0 || strcmp(pDirent->d_name, "..") == 0)
		{
			continue;
		}
		printf("pDirent->d_name:%s\n", pDirent->d_name);
		memset(chFilePathName, 0, sizeof(chFilePathName));

		snprintf(chFilePathName, sizeof(chFilePathName), "./ProxyLog/%s", pDirent->d_name);
		printf("chFilePathName:%s \n", chFilePathName);
		if (pDirent->d_type == DT_DIR) 
		{
			printf("is dir...\n");
		}

		if (lstat(chFilePathName, &rStat) == 0)
		{
			printf("sss \n");
			// 文件已经到达失效时间
			printf("ntime:%lu  rStat.st_mtim.tv_sec:%lu\n", nTime, rStat.st_mtim.tv_sec);
			printf("delta:%lu \n", nTime - rStat.st_mtim.tv_sec);
            // rStat.st_mtim.tv_sec 最后修改时间 
			if ((nTime - rStat.st_mtim.tv_sec) > 1 * 3600 * 9 + 35*60)
			{
				//
				printf("满足条件 \n");
				int ret = rm_dir(chFilePathName);
				//int ret = rmdir(chFilePathName);
				printf("ret:%d errno:%d", ret, errno);
			}
		}
	}
#endif 

	return 0;
}
