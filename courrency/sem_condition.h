#ifndef SEM_CONDITION_H
#define SEM_CONDITION_H

#include <thread>
#include <condition_variable>

class sem_condition
{
public:
    sem_condition(int init):v(init){}
    
    void P();
    void V();

private:
    std::mutex mutex_v; 
    std::condition_variable c_v;
    int v;
};

#endif