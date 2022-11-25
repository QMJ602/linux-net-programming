#ifndef LOCKER_H
#define LOCKER_H
#include <exception>
#include <pthread.h>
#include <semaphore.h>  //posix信号量

//posix信号量类
class sem{
public:
    sem()
    {
        if(sem_init(&m_sem, 0, 0) != 0)
        {
            throw std::exception();  //抛出异常，报告错误
        }
    }
    ~sem()
    {
        sem_destroy(&m_sem);
    }
    //等待信号量
    bool wait()
    {
        return sem_wait(&m_sem) == 0;//信号量减一，若值为0，将阻塞，直到信号量非0
    }
    //增加信号量
    bool post()
    {
        return sem_post(&m_sem) == 0;//信号量加一，当信号量大于0时，其他正在阻塞的sem_wait将被唤醒
    }
private:
    sem_t m_sem;  //信号量
};

//互斥锁类
class locker{
public:
    locker()
    {
        if(pthread_mutex_init(&m_mutex, NULL) != 0)//创建并初始化互斥锁
        {
            throw std::exception();
        }
    }
    ~locker()
    {
        pthread_mutex_destroy(&m_mutex);//销毁
    }
    bool lock()
    {
        return pthread_mutex_lock(&m_mutex) == 0; //加锁，若已被锁定，将阻塞
    }
    bool unlock()
    {
        return pthread_mutex_unlock(&m_mutex) == 0; //解锁
    }
private:
    pthread_mutex_t m_mutex;//互斥锁
};

//条件变量类
class cond{
public:
    cond()
    {
        if(pthread_mutex_init(&m_mutex, NULL) != 0)//创建并初始化互斥锁
        {
            throw std::exception();
        }
        if(pthread_cond_init(&m_cond, NULL) != 0)
        {
            pthread_mutex_destroy(&m_mutex);
            throw std::exception();
        }
    }
    ~cond()
    {
        pthread_cond_destroy(&m_cond);
        pthread_mutex_destroy(&m_mutex);
    }
    //等待条件变量
    bool wait()
    {
        int ret = 0;
        pthread_mutex_lock(&m_mutex);
        ret = pthread_cond_wait(&m_cond, &m_mutex); //调用时将自动释放互斥锁，返回时给互斥锁加锁
        pthread_mutex_unlock(&m_mutex);
        return ret == 0;
    }
    //唤醒等待条件变量的线程
    bool signal()
    {
        return pthread_cond_signal(&m_cond) == 0;
    }
private:
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
};

#endif