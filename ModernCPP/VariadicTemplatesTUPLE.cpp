#include <iostream>
#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED

#include <boost/stacktrace.hpp>
#include <boost/type_index.hpp>
using namespace std;
using boost::typeindex::type_id_with_cvr;



template<int I, typename Head, typename... Args>
struct Getter;

template <typename... Tail>
class Tuple{};

template<>
struct Tuple<>{};


template<typename Head, typename... Tail>
struct Tuple<Head, Tail...>  : Tuple<Tail...> // развернется в
        // Tuple<int, double, string>  : Tuple<double, string>
        // Tuple<double, string>       : Tuple<string>
        // Tuple<string>               : Tuple<>
{
    Tuple(Head h, Tail... tail)
            : Tuple<Tail...>(tail...), _head(h)
    {}

    template<int I>
    decltype(auto) get(){
        Getter<I, Head, Tail...> getter;
        return getter.get(*this);
    }

    using base_type = Tuple<Tail...>;
    // нужен для примера, чтобы был доступ к данным
    // на самом деле не нужен, все делает get
    base_type& base = static_cast<base_type&>(*this);

    Head       _head;
};


template<int I, typename Head, typename... Args>
struct Getter
{
    using return_type = typename Getter<I - 1, Args...>::return_type;
    static return_type get(Tuple<Head, Args...> &t)
    {
        return Getter<I - 1, Args...>::get(t);
    }
};

template<typename Head, typename... Args>
struct Getter<0, Head, Args...>{

    using return_type = decltype(Tuple<Head, Args...>::_head);
    static return_type get(Tuple<Head, Args...> t)
    {
        return t._head;
    }

};


int main(){
    Tuple<int, double, string> mytuple(1, 2.5, string("kek"));
    // можно образаться так, но лучше через Getter<>
    cout << mytuple.base.base._head.c_str() << endl;
    cout << mytuple.get<2>() << endl;

    // не компилятся. Args кончаются быстрее чем I
    // cout << mytuple.get<-1>() << endl;
    // cout << mytuple.get<5>() << endl;
    return 0;
}