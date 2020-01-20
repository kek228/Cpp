#include <iostream>
#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;


/* ****** template type deducing ****** */
// T->type, T&->param type (тип со всеми модификаторами итд)
template<typename T>
void f(T& param){
    std::cout<<type_id_with_cvr<T>().pretty_name();
}

int main() {
    int x = 1;
    const int refx = x;
    const int &crefx = x;

    f(refx);
    return 0;
}
