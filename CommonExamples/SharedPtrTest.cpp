#include "SharedPtr.h"
#include <memory>
#include <vector>
#include <thread>
#include <iostream>

void f(int *ptr) {
    delete ptr;
    //return true;
}

struct Test{
    Test():v(1){
        std::cout<<"Test()"<<std::endl;
    }

    explicit Test(int val):v(val){
        std::cout<<"Test(int val)"<<std::endl;
    }

    ~Test(){
        std::cout<<"~Test() val = "<<v<<std::endl;
    }

    int v;
};
#include <chrono>
using namespace std::chrono_literals;

void tfunc(SharedPtr<std::vector<int>> ptr) {
    for(int i = 0 ; i < 1000; ++i) {}
}

void multitheadTest() {
    auto deleter = [](std::vector<int> *ptr){
        std::cout<<(*ptr)[2]<<std::endl;
        delete ptr;
    };

    SharedPtr<std::vector<int>> origP(new std::vector<int>{1, 2, 3, 4, 5}, deleter);
    std::vector<std::thread> threads;
    for(int i = 0; i < 30; ++i)
        threads.emplace_back(tfunc, origP);

    origP.reset(nullptr);
    for(auto &t: threads)
        t.join();
    int a = 1;
}

int main() {
    SharedPtr<Test> stptr(new Test());
    //
    auto deleter = [](Test *ptr){
        std::cout<<"deleter"<<std::endl;
        delete ptr;
    };
    SharedPtr<Test> stptr2(new Test(10), deleter);

    auto deleter2 = [](Test *ptr){
        std::cout<<"shared_ptr deleter"<<std::endl;
        delete ptr;
    };
    std::shared_ptr<Test> ptr(new Test, deleter2);
    for(int i = 0; i < 100000; ++i) {
        std::cout<<"TEST=="<<i<<std::endl;
        multitheadTest();
    }

    return 0;
}