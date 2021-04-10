#include <iostream>
#include <thread>
#include <deque>
#include <condition_variable>
#include "sem_condition.h"

using namespace std;

const int MaxProduce=5;
const int MaxCustomer=5;
const int MaxQueueNum=5;
deque<int> q;
int data = 0;

mutex gc,g_common;
condition_variable ck;
sem_condition mutex_common(1);
sem_condition item_produce(MaxQueueNum),item_customer(0);

void produce_sem(){
    while (true){
        item_produce.P();
        mutex_common.P();
        data++;
        q.push_back(data);
        printf("Produce:: %d,Current size: %d \n",data,q.size());
        mutex_common.V();
        item_customer.V();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void customer_sem(){
    while (true){
        item_customer.P();
        mutex_common.P();
        printf("Customer:: %d,Current size: %d \n",q.front(),q.size());
        q.pop_front();
        mutex_common.V();
        item_produce.V();
        this_thread::sleep_for(chrono::milliseconds(500));
    }

}

void produce(){
    while (1){
        //this_thread::sleep_for(chrono::milliseconds(500));
        // lock gc;
        unique_lock<mutex> ck_lk(gc);
        ck.wait(ck_lk,[]()->bool{return q.size()<MaxQueueNum;});
        data++;
        q.push_back(data);
        printf("Produce:: %d,Current size: %d \n",data,q.size());
        ck_lk.unlock();
        ck.notify_all();
    }
}

void customer(){
    while (1){
        //this_thread::sleep_for(chrono::milliseconds(500));
        // lock gc;
        unique_lock<mutex> ck_lk(gc);
        ck.wait(ck_lk,[]()->bool{return !q.empty();});
        printf("Customer:: %d,Current size: %d \n",q.front(),q.size());
        q.pop_front();
        ck_lk.unlock();
        ck.notify_all();
    }
}

int main()
{
    thread ProdceTh[MaxProduce];
    thread CustomerTh[MaxCustomer];

    for (int i =0;i<MaxProduce;i++){
        // move copy operation 
        ProdceTh[i] = thread(produce);
    }
    for(int i =0;i<MaxCustomer;i++){
        // move copy operation
        CustomerTh[i] = thread(customer);
    }
    for (auto&item : ProdceTh){
        item.join();
    }
    for (auto& item:CustomerTh){
        item.join();
    }

}