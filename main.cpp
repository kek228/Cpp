#include <iostream>
using namespace std;

void f(int x){
    static int y = 10;
    y *= x;
    cout<<y<<endl;
}

int main(){
    for(int i = 0; i < 5; ++i)
        f(i + 1);
}