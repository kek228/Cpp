#include "SharedPtr.h"
#include <memory>
#include <iostream>

void f(int *ptr){
    delete ptr;
    //return true;
}
int main(){
    int* a = new int(100);
    SharedPtr<int> aptr(a);
    return 0;
}