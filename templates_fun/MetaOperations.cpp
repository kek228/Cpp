#include <iostream>
using namespace std;
// Метапрограммирование это серьезная и систематическая вещь
// начем с определения:
// МЕТАФУНКЦИЯ: это КЛАСС который определяет либо ТИП T или целое значение N
// В метапрограммировании есть тип данных: список

// объявление
template<typename ... Types>
struct TypeList;
// откусываем голову и рекурсивно обрабатываем хвост
template <typename Head, typename ... Tail>
struct TypeList <Head, Tail ...> {
    using ListHead = Head;
    using ListTail = TypeList <Tail ...>;
};

// дно рекурсии
template<>
struct TypeList <> {

};

// операции над списками:
// 1. длинна списка
template <typename Head, typename ... Tail>
struct GenericListLen {
    static const int length = 1 + GenericListLen<Tail ...>::length;
};

template <typename LastEl>
struct GenericListLen<LastEl> {
    static const int length = 1;
};
//

// Длинна для TypeList
template< typename TL >
struct TypeListLen {
    // f(list) = 1 + f(list[1:n])
    static const int length = 1 + TypeListLen<typename TL::ListTail>::length;
};

template<typename LastType>
struct TypeListLen<TypeList<LastType>> {
    static const int length = 1;
};

// Зная list и length попробуем сделать чуть другой tuple, без наследования
template<typename ... Types>
struct Tuple;

template<>
struct Tuple<> {

};

template<typename LastType>
struct Tuple<LastType> {
    Tuple(LastType head): _head(head){}
    using HEAD = LastType;
    HEAD _head;
    using TAIL = Tuple<>;
    TAIL _tail;
};


template<typename Head, typename ... Tail>
struct Tuple<Head, Tail ...> {
    using HEAD = Head;
    Head _head;
    using TAIL = Tuple<Tail ...>;
    TAIL _tail;
    Tuple(Head head, Tail ... tail) : _head(head), _tail(tail ...) {}

};

// теперь нужен get
template<int N, typename TUPLE>
struct Getter {
    using returnType = typename Getter<N-1, typename TUPLE::TAIL>::returnType;

    static returnType get(TUPLE &t) {
        return Getter<N-1, decltype(t._tail)>::get(t._tail);
    }
};

template<typename TUPLE>
struct Getter<0, TUPLE> {
    using returnType = typename TUPLE::HEAD;

    static returnType get(TUPLE &t) {
        return t._head;
    }
};

int main() {
    Tuple<int, double, char> myTuple(1, 3.14, 'p');
    Getter<2, decltype(myTuple)> myGetter;
    cout<<myGetter.get(myTuple);
    return 0;
}