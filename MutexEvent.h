#ifndef _MUTEXEVENT_H_
#define _MUTEXEVENT_H_

#include "MutexLock.h"
#include "MutexScopedLock.h"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// Description:    封装boost的condition_variable_any形成事件类

class CMutexEvent
{
public:
    CMutexEvent(void)
    {
    }
    virtual ~CMutexEvent(void)
    {
    }
    
public:
    /*
    * Function:    void event_wait(void)
    * Description: 等待事件
    * Input:       none
    * Output:      none 
    * Return:      none
    * Others:      none
    */
    void event_wait(void)
    {
        CMutexScopedLock scoped_lock(m_mutex_lock);
        m_condition_var.wait(scoped_lock);        
    }
    
    /*
    * Function:    bool event_wait_with_time(int nmill_sec)
    * Description: 通过设定等待时间改变事件
    * Input:       int nmill_sec：等待时间
    * Output:      none 
    * Return:      返回bool类型：等待时间内触发事件返回true，等待时间内未触发事件返回false
    * Others:      none
    */
    bool event_wait_with_time(int nmill_sec)
    {
        CMutexScopedLock scoped_lock(m_mutex_lock);
        return m_condition_var.timed_wait(scoped_lock, boost::posix_time::millisec(nmill_sec));
    }
    
    /*
    * Function:    void notify_one(void)
    * Description: 唤醒某个特定线程
    * Input:       none
    * Output:      none 
    * Return:      none
    * Others:      多个等待时不确定唤醒那个线程
    */
    void notify_one(void)
    { 
        m_condition_var.notify_one(); 
    }

    /*
    * Function:    void notify_one(void)
    * Description: 唤醒所有线程
    * Input:       none
    * Output:      none 
    * Return:      none
    * Others:      none
    */
    void notify_all(void)
    {
        m_condition_var.notify_all(); 
    }
    
private:
    CMutexLock m_mutex_lock;
    boost::condition_variable_any m_condition_var;
};

#endif //end _MUTEXEVENT_H_
