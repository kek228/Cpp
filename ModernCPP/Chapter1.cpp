#include <iostream>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;
using namespace std;
// ГЛАВА 1. Вывод типов для шаблонов, auto и decltype
/* ****** ITEM 1: template type deducing ****** */

// CASE1: param type: T&
// Тип T отбрасывает ссылку, но оставляет const/volatile
template<typename T>
void case1(T &param) {
    cout << type_id_with_cvr<T>().pretty_name() << std::endl;
}

// CASE2: param type: T&& (универсальная ссылка)
// Все предсказуемо, T теперь просто не отбросит ссылку
// НО отдельный кейс когда param type: rvalue, T разрезолвится в int
template<typename T>
void case2(T &&param) {
    cout << type_id_with_cvr<T>().pretty_name() << std::endl;
}

// CASE3: param type: T
// вся ссылочность и константность отбросится, ведь param будет копией
template<typename T>
void case3(T param) {
    cout << type_id_with_cvr<T>().pretty_name() << std::endl;
}

void item1() {
    int x = 1;
    const int cx = 2;
    const int &crefx = x;

    // CASE 1: как видно & нигде нет, потому что pt и так ссылка
    cout << "template<typename T>\n"
            "void case1(T& param){}" << endl;
    case1(x); // int
    case1(cx); // int const
    case1(crefx); // int const

    // CASE 2: теперь & везде есть кроме rvalue, потому что универсальная ссылка должна в что то разрезолвится
    cout << "template<typename T>\n"
            "void case2(T&& param){}" << endl;
    case2(x); // int&
    case2(cx); // int const&
    case2(crefx); // int const&
    case2(100); // int

    // CASE 3
    cout << "template<typename T>\n"
            "void case3(T param){}" << endl;
    case3(x); // int
    case3(cx); // int
    case3(crefx); // int
    case3(100); // int
    const char *const ptr = "Fun with pointers";
    case3(ptr); // char const* вторая константность будет отброшена, ptr скопируется и его можно будет занулить
    // но не первая, ведь данные по этому адресу менять НЕЛЬЗЯ

    // Array arg. Что будет если передать T[] или T*
    cout << "ARRAY ARG" << endl;
    char arr[] = "hello";
    char *ptr_to_arr = arr;
    case1(arr); // char [6] ссылочность была отброшена, а тип остался, как и для int-a
    case1(ptr_to_arr); // char*
    case2("kek"); // char const (&) [4] // оба ссылки на arr[]
    case2(arr); // char (&) [6]
    case2(ptr_to_arr); // char*& какое то очко
    case3(arr); // char* тут все понятно, это просто скопировался поинтер
    case3(ptr_to_arr); // char*
}
/***** ITEM 2 *****/
// А теперь тоже самое только для auto
// Смысл, что auto это как T в функциях case сверху, а Pt это cv с auto и &/*
// case-ы те же самые выводится тип auto точно также, КРОМЕ случая auto a = {}
// там auto будет std::initializer_list
void item2() {
    cout << "ITEM 2" << endl;
    int x = 1;
    const int cx = 2;
    const int &crefx = x;
    // CASE 1 auto развернется как ПРОСТО int и оставит cv, в итоге будет:
    {
        auto &auto_x = x; // int&
        auto &auto_cx = cx; // const int&
        auto &auto_crefx = crefx; // const int&
    }
    // CASE 2
    {
        auto &&auto_x = x; // int&
        auto &&auto_cx = cx; // const int&
        auto &&auto_crefx = crefx; // const int&
        auto &&rval = 1; // int &&
    }
    // CASE 3 везде тупо int
    {
        auto auto_x = x; // int
        auto auto_cx = cx; // int
        auto auto_crefx = crefx; // int
        auto rval = 1; // int
    }

}

/* ITEM3 decltype */
void authenticateUser() {}

template<typename Container, typename Index>
// C++14;
auto authAndAccess1(Container &c, Index i)
// тип в auto выведется в int (как в выражении auto a = crefx)
// значит вернется КОПИЯ, а мы то хоти ССЫЛКУ
{
    authenticateUser();
    return c[i]; // return type deduced from c[i]
}

// Рефакторинг authAndAccess:
// decltype(auto) теперь возвращаемый тип точно такой как у c[i]
template<typename Container, typename Index>
// C++14; works,
decltype(auto) authAndAccess2(Container &c, Index i) {
    authenticateUser();
    return c[i];
}

// Дальнейший рефакторинг: переход на универсальную ссылку
// Тип возвращаемого значения не поменялся, но теперь можно вызывать с rval, например для копирования элемента
template<typename Container, typename Index>
decltype(auto) authAndAccess3(Container &&c, Index i) {
    authenticateUser();
    return forward<Container>(c)[i];
}

// ебанутая особенность
decltype(auto) returnx(){// вернет int
    int x = 1;
    return x;
}

decltype(auto) returnx1(){// вернет int&
    int x = 2;
    return (x);
}


void item3() {
    cout << "ITEM 3" << endl;
    // Как правило decltype тупо возвращает точный тип выражения, которое ему отдали
    // Вот тут b такой же как a
    const int a = 1;
    decltype(a) b = 2; // const int
    static_assert(std::is_same<decltype(a), decltype(b)>::value, "a and b are not the same");

    // Основной профит decltype в способности писать функции,
    // чей тип возвращаемого значения зависит от параметров шаблона
    // рассмотрим authAndAccess
    vector<int> cont = {1, 2, 3, 4};
    auto x1 = authAndAccess1(cont, 2);
    cout << "X1 type: " << type_id_with_cvr<decltype(authAndAccess1(cont, 2))>().pretty_name() << std::endl; //int
    auto x2 = authAndAccess2(cont, 2);
    auto &x3 = authAndAccess2(cont, 2);
    x3 = 100500;
    cout << "cont[2] new val = " << cont[2] << endl; // cont = {1, 2, 100500, 4}
    cout << "X2 type: " << type_id_with_cvr<decltype(authAndAccess2(cont, 2))>().pretty_name() << std::endl; //int
    cout<<"authAndAccess3"<<endl;
    cout << "X3 type: " << type_id_with_cvr<decltype(authAndAccess3(cont, 2))>().pretty_name() << std::endl;
    cout << "X3 type with rval: " << type_id_with_cvr<decltype(authAndAccess3(vector<int>{1}, 0))>().pretty_name() << std::endl;
    cout<<"return x and return (x)"<<endl;
    cout << type_id_with_cvr<decltype(returnx())>().pretty_name() << std::endl;
    cout << type_id_with_cvr<decltype(returnx1())>().pretty_name() << std::endl;
}


int main() {
    item1();
    item2();
    item3();
    return 0;
}
