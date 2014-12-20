#ifndef _DATAQUEUE_H_
#define _DATAQUEUE_H_

#include "MutexLock.h"
#include "MutexScopedLock.h"
#include <deque>
#include <boost/make_shared.hpp>

// Description:    基于双端队列实现的通用模板队列

#ifndef _STL_DEFAULT_TEMPLATES_  
template <class T, class Sequence = std::deque<T> > 
#else  
template <class T, class Sequence>
#endif  

class CDataQueue
{
public:
    CDataQueue(void)
    {
        m_ptr_mutex = boost::make_shared<CMutexLock>();
    }
        
    virtual ~CDataQueue(void)
    {
        clear();
    }
    
public:
    /*
    * Function:    void push(T &t, bool bprior)
    * Description: 入队操作
    * Input:       T &t：模板类，bprior (true:优先，false:不优先)
    * Output:      none 
    * Return:      none
    * Others:      如果优先则在对头入队，否则在队尾入队
    */
    void push(T &t, bool bprior = false)
    {
        CMutexScopedLock scoped_lock(*m_ptr_mutex);
        if (bprior)
        {
            m_sequeue.push_front(t);
        }
        else
        {
            m_sequeue.push_back(t);
        }
    }
    
    /*
    * Function:    bool pop(T &t)
    * Description: 出队操作
    * Input:       T &t：模板类
    * Output:      none 
    * Return:      返回bool类型：出队成功返回true，出队失败返回false
    * Others:      队头出队
    */
    bool pop(T &t)
    {
        CMutexScopedLock scoped_lock(*m_ptr_mutex);
        if(!m_sequeue.empty())
        {
            t = m_sequeue.front();
            m_sequeue.pop_front(); 
            return true;
        }
        
        return false;
    }
    
    /*
    * Function:    int size(void)
    * Description: 队列大小
    * Input:       none
    * Output:      none 
    * Return:      返回int类型：队列大小
    * Others:      none
    */
    int size(void)
    {
        CMutexScopedLock scoped_lock(*m_ptr_mutex);
        return m_sequeue.size();
    }
    
    /*
    * Function:    bool empty(void)
    * Description: 队列是否为空
    * Input:       none
    * Output:      none 
    * Return:      返回bool类型：队列不为空返回true，队列为空返回false
    * Others:      none
    */
    bool empty(void)
    {
        CMutexScopedLock scoped_lock(*m_ptr_mutex);
        if(!m_sequeue.empty())
        {
            return true;
        }
        
        return false;
    }
    
    /*
    * Function:    void empty(void)
    * Description: 清空队列
    * Input:       none
    * Output:      none 
    * Return:      none
    * Others:      none
    */
    void clear(void)
    {
        CMutexScopedLock scoped_lock(*m_ptr_mutex);
        m_sequeue.clear();
    }

protected:
    //智能指针
    boost::shared_ptr <CMutexLock> m_ptr_mutex;
    //维护的容器 
    Sequence m_sequeue;   
};

#endif //end _DATAQUEUE_H_
