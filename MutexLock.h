#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

#include <boost/thread.hpp>

// Description:    封装boost的mutex类

class CMutexLock
{
public:
    CMutexLock(void)
    {
    }
    virtual ~CMutexLock(void)
    {
    }
    
public:
    /*
    * Function:    virtual void lock(void)
    * Description: 锁定
    * Input:       none
    * Output:      none 
    * Return:      none
    * Others:      none
    */
    virtual void lock(void)
    {
        m_mutex.lock();
    }
    
    /*
    * Function:    virtual void unlock(void)
    * Description: 解锁
    * Input:       none
    * Output:      none 
    * Return:      none
    * Others:      none
    */
    virtual void unlock(void)
    {
        m_mutex.unlock();
    }
    
protected:
    boost::mutex m_mutex; 
};

#endif //end _MUTEXLOCK_H_
