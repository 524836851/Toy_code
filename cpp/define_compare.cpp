#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
using namespace std;
bool compare(const int&a,const int&b){
    return a>b;
}
priority_queue<int,vector<int>,decltype(compare)*> k([](const int&a,const int&b)->bool{return a>b;});
priority_queue<int,vector<int>,bool(*)(const int&,const int&)> kk([](const int&a,const int&b)->bool{return a>b;});
priority_queue<int,vector<int>,greater<int> > kkk;

int main()
{
    for (int i =0;i<10;i++){
        kk.push(i);
    }
    while (!kk.empty()){
        cout << kk.top() << endl;
        kk.pop();
    }
}