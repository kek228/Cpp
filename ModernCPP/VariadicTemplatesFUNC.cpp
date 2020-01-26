#include <iostream>

#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED

#include <boost/stacktrace.hpp>
#include <boost/type_index.hpp>

using namespace std;
using boost::typeindex::type_id_with_cvr;

// Variadic Templates
// Это либо функция с переменным числом шаблонных аргументов
template<typename... Args>
void foo(Args... args);
// Либо класс
template<typename... Args>
struct some_type;
// Что делать с этим списком?
// Прием 1. Args можно использовать в качестве аргумета вызова функции:
// применять к нему операции каста и т.п. При этом раскрывается он в
// зависимости от положения ... , а именно, раскрывается выражение непосредственно прилегающее к ... слева.

// Прием 2. Можно откусывать первый параметр в Args, вызовом  <typename HEAD, typename... Args>

// Прием 3. Можно получить размер списка вызовом sizeof...(args)

// Пример 1:
template <typename HEAD>
void example1(HEAD& arg){
    cout<<"HEAD type = " << type_id_with_cvr<HEAD>().pretty_name()<<endl;
    cout<<arg<<endl;
}

template <typename HEAD, typename... Args>
void example1(HEAD& arg, Args... args){
    cout << "sizeof...(args) == " << sizeof...(args) << endl;
    cout<<"HEAD type = " << type_id_with_cvr<HEAD>().pretty_name()<<endl;
    cout<<arg<<endl;
    example1((args)...);
}

template<typename... Args>
void example1(Args... args){
    cout << "sizeof...(args) == " << sizeof...(args) << endl;
    example1((args)...); // раскроется в foo<int, typename... Args>(1, 2.3, "kek");
}

// Применить к args можно применить функцию, const, move/forward и прочие модификации типа
// (const args&..)           -> (const T1& arg1, const T2& arg2, ...)
// ((f(args) + g(args))...)  -> (f(arg1) + g(arg1), f(arg2) + g(arg2), ...)
// (f(args...) + g(args...)) -> (f(arg1, arg2,...) + g(arg1, arg2, ...))
// (std::make_tuple(std::forward<Args>(args)...))
//                           -> (std::make_tuple(std::forward<T1>(arg1), std::forward<T2>(arg2), ...))

template <typename HEAD>
void example2(HEAD&& head){
    cout<<"HEAD type = " << type_id_with_cvr<HEAD>().pretty_name()<<endl;
}

template <typename HEAD, typename... Args>
void example2(HEAD&& head, Args&&... args){
    cout<<"HEAD type = " << type_id_with_cvr<HEAD>().pretty_name()<<endl;
    cout<<"HEAD forward type = " << type_id_with_cvr<decltype(forward<HEAD>(head)) >().pretty_name()<<endl;
    example2(forward<Args>(args)...);
}

template<typename... Args>
void example2(Args&&... args){
    example2(forward<Args>(args)...); // раскроется в foo<int, typename... Args>(1, 2.3, "kek");
}

int main() {
    example1(1, 2.3, "kek"); // инстанцируется в foo<int, double, char const*>(1, 2.3, "kek")
    cout<<"EXAMPLE 2:"<<endl;
    int first = 1;
    double third = 3;
    const auto &rf = first;
    example2(first, string("kek"), third, rf);
}