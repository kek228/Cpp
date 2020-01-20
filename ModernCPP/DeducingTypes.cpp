#include <iostream>
#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;
using namespace std;
// ГЛАВА 1. Вывод типов для шаблонов, auto и decltype
/* ****** ITEM 1: template type deducing ****** */

// CASE1: param type: T&
// Тип T отбрасывает ссылку, но оставляет const/volatile
template<typename T>
void case1(T& param){
    cout<<type_id_with_cvr<T>().pretty_name()<<std::endl;
}

// CASE2: param type: T&& (универсальная ссылка)
// Все предсказуемо, T теперь просто не отбросит ссылку
// НО отдельный кейс когда param type: rvalue, T разрезолвится в int
template<typename T>
void case2(T&& param){
    cout<<type_id_with_cvr<T>().pretty_name()<<std::endl;
}

// CASE3: param type: T
// вся ссылочность и константность отбросится, ведь param будет копией
template<typename T>
void case3(T param){
    cout<<type_id_with_cvr<T>().pretty_name()<<std::endl;
}

void item1(){
    int x = 1;
    const int cx = 2;
    const int &crefx = x;

    // CASE 1: как видно & нигде нет, потому что pt и так ссылка
    cout<<"CASE1"<<endl;
    case1(x); // int
    case1(cx); // int const
    case1(crefx); // int const

    // CASE 2: теперь & везде есть кроме rvalue, потому что универсальная ссылка должна в что то разрезолвится
    cout<<"CASE2"<<endl;
    case2(x); // int&
    case2(cx); // int const&
    case2(crefx); // int const&
    case2(100); // int

    // CASE 3
    cout<<"CASE3"<<endl;
    case3(x); // int
    case3(cx); // int
    case3(crefx); // int
    case3(100); // int
    const char* const ptr = "Fun with pointers";
    case3(ptr); // char const* вторая константность будет отброшена, ptr скопируется и его можно будет занулить
    // но не первая, ведь данные по этому адресу менять НЕЛЬЗЯ
}



int main() {
    item1();
    return 0;
}
