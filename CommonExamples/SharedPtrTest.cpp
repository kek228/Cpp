#include "SharedPtr.h"
#include <memory>
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



int main() {
    SharedPtr<Test> stptr(new Test());
    //
    auto deleter = [](Test *ptr){
        std::cout<<"deleter"<<std::endl;
        delete ptr;
    };
    SharedPtr<Test> stptr2(new Test(10), deleter);

    return 0;
}