#ifndef _MUTEXSCOPEDLOCK_H_
#define _MUTEXSCOPEDLOCK_H_

#include "MutexLock.h"

// Description:    封装boost的ScopedLock类：发生异常时能自动解锁，避免线程死锁情况

class CMutexScopedLock : public CMutexLock
{
public:
    CMutexScopedLock(CMutexLock &mutex_lock)
    {
        m_pmutex_lock = &mutex_lock;
        m_pmutex_lock->lock();
    }
    
    virtual ~CMutexScopedLock(void)
    {
        m_pmutex_lock->unlock();
    }
    
public:
    /*
    * Function:    virtual void lock(void)
    * Description: 锁定
    * Input:       none
    * Output:      none 
    * Return:      锁定状态
    * Others:      none
    */
    virtual void lock(void)
    {
        return m_pmutex_lock->lock();
    }
    
    /*
    * Function:    virtual void unlock(void)
    * Description: 解锁
    * Input:       none
    * Output:      none 
    * Return:      解锁状态
    * Others:      none
    */
    virtual void unlock(void)
    {
        return m_pmutex_lock->unlock();
    }
    
private:
    CMutexLock *m_pmutex_lock;
    
};

#endif //end _MUTEXSCOPEDLOCK_H_
