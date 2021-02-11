#include <iostream>
#include <atomic>
using namespace std;


int main(){
    int * pp = new int(10500);
    atomic<int *> p(pp);
    cout<<*p;
}