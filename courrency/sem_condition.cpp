#include "sem_condition.h"


void sem_condition::P(){
    // lock
    std::unique_lock<std::mutex> lk(mutex_v);
    c_v.wait(lk,[this]()->bool{return this->v>0;});
    this->v--;
    lk.unlock(); 
    c_v.notify_all();
}

void sem_condition::V(){
    std::unique_lock<std::mutex> lk(mutex_v);
    this->v++;
    lk.unlock();
    c_v.notify_all();
}