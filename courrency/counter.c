#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#define ThreadNum 6
int counter=0;
const int MaxNum = 1000000;
const int eps = 1024;

typedef struct counterP
{
    int counter;
    pthread_mutex_t global;
    int llocal[ThreadNum];
    pthread_mutex_t llock[ThreadNum];
    /* data */
} counterP;

counterP counter2;

void init(counterP* p){
    p->counter=0;
    memset(p->llocal,0,sizeof(p->llocal));
    for (int i =0;i<ThreadNum;i++){
        pthread_mutex_init(&p->llock[i],NULL);
    }
}

void add(counterP*p,int tNum){
    //pthread_mutex_lock(&p->llock[tNum]);
    p->llocal[tNum]++;
    if (p->llocal[tNum]> eps){
        pthread_mutex_lock(&p->global);
        p->counter+= p->llocal[tNum];
        p->llocal[tNum]=0;
        pthread_mutex_unlock(&p->global);
    }
    //pthread_mutex_unlock(&p->llock[tNum]);
}

pthread_mutex_t counter_lock;

void time_spend(void (*func)(),char* name)
{
    clock_t beg,end;
    beg = clock();
    func();
    end = clock();
    printf("%s spends time: %lf\n",name,(double)(end-beg)/CLOCKS_PER_SEC * 1000);
}

void work_serial(){
    for (int i =0;i<ThreadNum;i++){
        for (int j=0;j<MaxNum;j++){
            counter++;
        }
    }
    printf("%d\n",counter);
}
void* work_single_2(void* tnum){
    for (int i =0;i<MaxNum;i++){
        add(&counter2,(int)tnum);
    }
}

void* work_single(){
    for (int i =0;i<MaxNum;i++){
        pthread_mutex_lock(&counter_lock);
        counter++;
        pthread_mutex_unlock(&counter_lock);
    }
}

void work_parallel(){
    pthread_t pid[ThreadNum];      
    int rc;
    for (int i =0;i<ThreadNum;i++){
        rc = pthread_create(pid+i,NULL,work_single,NULL);
    }
    for (int i =0;i<ThreadNum;i++){
        pthread_join(pid[i],NULL);
    }
    printf("%d\n",counter);
}

void work_parallel_2(){
    init(&counter2);
    pthread_t pid[ThreadNum];
    int rc;
    for (int i =0;i<ThreadNum;i++){
        rc = pthread_create(pid+i,NULL,work_single_2,i);
    }
    for (int i =0;i<ThreadNum;i++){
        pthread_join(pid[i],NULL);
    }
    printf("%d\n",counter2.counter);

}

int main()
{
    time_spend(work_serial,"work_serial");
    counter=0;
    time_spend(work_parallel,"work parllel");
    time_spend(work_parallel_2,"work parallel 2");
}