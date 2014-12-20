#ifndef _MSGDATAHANDLER_H_
#define _MSGDATAHANDLER_H_

#include "DataQueue.h"
#include "MutexEvent.h"
#include <boost/thread.hpp>
#include <iostream>

using namespace std;

// Description:    封装线程间的数据消息处理类
//                 CMsgDataHandler属于基类，一般使用时，只需继承该类并重写virtual void data_handler(T& t)方法即可
//                 如果需要设置线程数则set_thread_num(默认为1)，如果需要设置缓冲区大小则set_threshold_size(默认为无限制无需设置)
//                 如果对数据发送有特殊处理，则需要重写data_thread线程方法

/*
example:

class CDataHandler : public CMsgDataHandler <int>
{
public:
    CDataHandler(){};
    virtual ~CDataHandler(){};
    
    virtual void data_handler(int &n)
    {
        cout << "data_handler = " << n << "\n" << endl;
    }
};

int main()
{
    CMsgDataHandler<int> *pdata_handler = new CDataHandler;
    int nval = 0;
    pdata_handler->set_thread_num(10);
    //pdata_handler->set_threshold_size(0);
    while(true)
    {
        pdata_handler->put(++nval);
        //等待一秒
        boost::this_thread::sleep(boost::posix_time::seconds(1));  
        cout << "this_thread = " << nval << "\n" << endl; 
        if (nval == 106)
        {
            break;
        }
    }
    cout << "the end ..." << "\n" << endl;
    if (pdata_handler != NULL)
    {
        delete pdata_handler;
        pdata_handler = NULL;
    }
}
*/

template <class T> 
class CMsgDataHandler : public CDataQueue <T>, public CMutexEvent
{
public:
    /*
    * Function:    CMsgDataHandler(int nthread_num = 1, bool bauto_start = true, int nthreshold_size = 0)
    * Description: 构造函数
    * Input:       int nthread_num：线程数目，默认为1，bool bauto_start：是否自动开启线程，默认为开启，int nthreshold_size：内部缓冲区阈值，默认为无限制
    * Output:      none 
    * Return:      none
    * Others:      在继承CMsgDataHandler类时重写该函数
    */
    CMsgDataHandler(int nthread_num = 1, bool bauto_start = true, int nthreshold_size = 0) : m_max_thread_num(nthread_num), m_threshold_size(nthreshold_size), m_bstart(false)  
    {  
        if(bauto_start)  
        {  
            start();  
            m_bstart = true;  
        }  
    }  
  
    virtual ~CMsgDataHandler(void) 
    {
    }

public:
    /*
    * Function:    virtual void data_handler(T& t)
    * Description: 获取数据后的处理函数
    * Input:       T& t：模板
    * Output:      none 
    * Return:      none
    * Others:      在继承CMsgDataHandler类时重写该函数
    */
    virtual void data_handler(T& t) = 0;
    
    /*
    * Function:    void set_thread_num(int nthread_num)
    * Description: 设置线程数
    * Input:       int nthread_num：线程数目
    * Output:      none 
    * Return:      none
    * Others:      默认为1
    */    
    void set_thread_num(int nthread_num)
    {
        m_max_thread_num = nthread_num;
    }  
  
    /*
    * Function:    void set_threshold_size(int nthreshold_size)
    * Description: 设置队列缓冲区大小
    * Input:       int nthreshold_size：缓冲区大小
    * Output:      none 
    * Return:      none
    * Others:      默认为无限制
    */  
    void set_threshold_size(int nthreshold_size) 
    {
        m_threshold_size = nthreshold_size;
    }  
    
    /*
    * Function:    void put(T& t, bool bprior = false)
    * Description: 队列中添加数据
    * Input:       T& t：模板， bool bprior：是否优先级
    * Output:      none 
    * Return:      none
    * Others:      none
    */
    void put(T& t, bool bprior = false)
    {
        m_data_queue.push(t, bprior);
        
        //当数据缓冲区大于承载量，则清空
        if (m_threshold_size > 0 && m_data_queue.size() > m_threshold_size)
        {
            m_data_queue.clear();
        }
        else
        {
            //发送通知信号
            notify_one();
        }
    }
    
protected:
    /*
    * Function:    virtual void data_thread(void)
    * Description: 数据处理线程
    * Input:       none
    * Output:      none 
    * Return:      none
    * Others:      none
    */
    virtual void data_thread(void)
    {
        while(true)
        {
            //等待信号
            wait();
            while(!m_data_queue.empty())
            {
                T t;
                if(m_data_queue.pop(t))
                {
                    data_handler(t);
                }
            }
        }
    }

private:
    /*
    * Function:    void start(void)
    * Description: 线程开启函数
    * Input:       none
    * Output:      none 
    * Return:      none
    * Others:      none
    */
    void start(void)
    {
        if(!m_bstart)  
        {  
            for(int i = 0; i < m_max_thread_num; ++i)  
            {  
                boost::thread data_work_Thread(&CMsgDataHandler::data_thread, this);  
            }  
        }    
    }

private:
    //数据队列
    CDataQueue <T> m_data_queue;
    //最大线程数
    int m_max_thread_num;
    //缓冲区大小
    int m_threshold_size;
    //是否自启动
    bool m_bstart;
    
};

#endif //end _MSGDATAHANDLER_H_
