#include <iostream>
#include <functional>

int func(int& a,int& b,int&& c){
    b++;
    return a+b+c;
}

template<typename T>
void func_right(T&& k){
    std::cout << k++ << std::endl;
}

template<typename F,typename T1,typename T2>
void flip1(F f,T1&& t1,T2&& t2){
    f(std::forward<T2>(t2),std::forward<T1>(t1));
}

void func_test(int&& v1,int& v2){
    std::cout << v1 << " " << ++v2 << std::endl;
}

template<typename F,typename ...Args>
void func_wrapper(F f,Args&&... args){
    f(std::forward<Args>(args)...);
}

int main()
{
    using namespace std::placeholders;
    int k = 3;
    int i = 4;

    std::function<void(int&,int&&)> func_0;
    auto func1=std::bind(func,std::ref(k),_1,_2);
    std::cout << func1(i,3) << std::endl;
    std::cout << i << std::endl;
    k = 4;
    std::cout << func1(i,3) << std::endl;
    std::cout << i << std::endl;
    
    std::cout << "test tempate" << std::endl;
    int a = 3;
    flip1(func_test,a,42);
    //func_right(std::ref(a));
    std::cout << a << std::endl;
    func_wrapper(func_test,2,a);
    std::cout << a << std::endl;
    int k_ref = std::ref(a);
}
