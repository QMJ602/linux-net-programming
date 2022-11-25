#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <pthread.h>
#include <queue>
#include <exception>
#include "../locker/locker.h"


//定义给worker传递的参数
struct params{
    void* pool;
    int index;
};

template <typename T>
class threadpool{
public:
    threadpool(int thread_number = 8, int max_requests = 1000);//线程池中线程的数量， 请求队列中最多请求数
    ~threadpool();
    //往请求队列中添加任务
    bool append(T* request);
private:
    static void* worker(void* arg);//工作线程运行的函数
    void run(int index);//传递参数为 当前线程序号
private:
    int m_thread_number;//线程数
    int m_max_requests;//最大请求数
    pthread_t* m_threads;//存放线程池中的线程的数组
    std::queue<T*> m_request_queue;//请求队列
    locker m_queue_locker; //保护请求队列的互斥锁
    sem m_queue_stat;//信号量
    bool m_stop; //是否结束线程
    params* m_params;
};


//类模板成员函数的创建时机是在调用阶段，因此模板类的声明实现必须在一个文件中
template<typename T>
threadpool<T>::threadpool(int thread_number, int max_requests)
{
    m_thread_number = thread_number;
    m_max_requests = max_requests;
    m_stop = false;
    m_threads = NULL;
    m_params = new params[thread_number];
    if(m_thread_number <= 0 || m_max_requests <= 0)
    {
        throw std::exception();
    }
    m_threads = new pthread_t[thread_number];
    //创建thread_number个线程，并将它们设置为脱离线程
    for(int i=0;i<m_thread_number;++i)
    {
        printf("Create the %dth thread\n", i);
        m_params[i].pool = this;
        m_params[i].index = i;
        if(pthread_create(m_threads+i, NULL, worker, m_params+i) != 0)//如果创建失败
        {
            delete [] m_threads;
            delete [] m_params;
            throw std::exception();
        }
        if(pthread_detach(m_threads[i]) != 0)//主线程与子线程分离，子线程结束后资源会自动回收
        {
            delete [] m_threads;
            delete [] m_params;
            throw std::exception();
        }
    }
}

template<typename T>
threadpool<T>::~threadpool()
{
    delete [] m_threads;
    delete [] m_params;
    m_stop = true;
}

template<typename T>
bool threadpool<T>::append(T* request)
{
    //操作工作队列时加锁，因为它被所有线程共享
    m_queue_locker.lock();
    if(m_request_queue.size() >= m_max_requests)
    {
        m_queue_locker.unlock();
        return false;
    }
    m_request_queue.push(request);
    m_queue_locker.unlock();
    m_queue_stat.post();//成功添加任务时，信号量+1
    return true;
}

template<typename T>
void* threadpool<T>::worker(void* arg)//传入了this指针
{
    params* pp = (params*)arg;
    threadpool* pool = (threadpool*)pp->pool;
    int index = pp->index;
    pool->run(index);
    return pool;
}

template<typename T>
void threadpool<T>::run(int index)
{
    while(!m_stop)
    {
        m_queue_stat.wait();//等待信号量，即等待队列中有任务，若无任务将阻塞
        m_queue_locker.lock();
        if(m_request_queue.empty())
        {
            m_queue_locker.unlock();
            continue;
        }
        T* request = m_request_queue.front();
        m_request_queue.pop();
        m_queue_locker.unlock();
        if(request)
        {
            // printf("工作线程%d正在处理...\n", index);
            request->process();
        }
    }
}


#endif