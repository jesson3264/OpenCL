/** @file threadpool.h */

#ifndef __XTHREAD_THREAD_POOL_H__
#define __XTHREAD_THREAD_POOL_H__

/**
 * @brief 
 * @details
 */
namespace SCL
{

class SThreadPool
{
public:
    SThreadPool();
    ~SThreadPool();

    /** 完整性检查 */
    void IntegrityCheck();
    /** 启动 */
    void Startup();
    /** 关闭 */
    void Shutdown();

    /** 
     * @brief 
     * @return  true - suspend ourselves, and wait for a future task.
	 *          false - exit, we're shutting down or no longer needed.
    */
    bool ThreadExit(Thread* t);

    Thread* StartThread(ThreadBase* execution_target);

private:


};

}//namespace SCL
#endif 