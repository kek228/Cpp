#include <iostream>
using namespace std;

template<int N>
struct Fact {
    static constexpr int val = N * Fact<N-1>::val;
};

template<>
struct Fact<0> {
    static constexpr int val = 1;
};



int main() {
    Fact<3> f;
    cout<<f.val;
}