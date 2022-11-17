#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
void* mythread1(void*)
{
    int i;
    for(i=0;i<5;++i)
    {
        std::cout << "thread1" << std::endl;
        // sleep(2);
    }
}

void* mythread2(void*)
{
    int i;
    for(i=0;i<5;++i)
    {
        std::cout << "thread2" << std::endl;
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    pthread_t th1, th2;
    int res;
    res = pthread_create(&th1, NULL, mythread1, NULL);
    assert(res == 0);
    res = pthread_create(&th2, NULL, mythread2, NULL);
    assert(res == 0);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    return 0;
}