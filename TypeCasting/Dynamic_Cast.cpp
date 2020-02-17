#include <cstdint>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

// dynamic_cast < new-type > ( expression )

// Что можно конвертить:
// МОЖНО КОНВЕРТИТЬ ТОЛЬКО УКАЗАТЕЛИ И ССЫЛКИ
// 1. Можно скастовать T в const T точно такой же
// 2. Можно nullptr в expression скастовать в ptr на какой то T
// 3. Если expression это D*, а new-type B* (или ссылка). Безопасно скастит прям как
// static_cast для не виртуальных. Но можно и для виртуальных в одной иерархии
// 4. Если expression полиморфный а new-type void*, то вернет САМЫЙ DERIVED от expression
// 5. САМОЕ ВАЖНОЕ DOWNCAST ПРИВЕДЕНИЕ:
// expression B а new-type: D. Тут будет происходить динамическая проверка типа.
// Если expression реально D то все хорошо (downcast)
// Еще можно скастовать вбок
// Если вообще нельзя вылетит исключение

// Замечания:
// 1. ГЕНЕРИРУЕТ код
// 2. Кидает исключения

struct V {
    virtual void f() {};  // must be polymorphic to use runtime-checked dynamic_cast
};
struct A : virtual V {};

struct B : virtual V {
    B(V* v, A* a) {
        // casts during construction (see the call in the constructor of D below)
        dynamic_cast<B*>(v); // well-defined: v of type V*, V base of B, results in B*
        dynamic_cast<B*>(a); // undefined behavior: a has type A*, A not a base of B
    }
};
struct D : A, B {
    D() : B(static_cast<A*>(this), this) { }
};

struct Base {
    virtual ~Base() {}
};

struct Derived: Base {
    int n = 100500;
    virtual void name() {
        cout<<"name == Derived, n  == " << n<<endl;

    }
};

int main()
{
    D d; // the most derived object
    A& a = d; // upcast, dynamic_cast may be used, but unnecessary
    B& new_b = dynamic_cast<B&>(a); // sidecast B сбоку от A в ромбе
    D& new_d = dynamic_cast<D&>(a); // downcast


    Base* b1 = new Base;
    if(auto d = dynamic_cast<Derived*>(b1))
    {
        std::cout << "downcast from b1 to d successful\n";
        d->name(); // safe to call
    }else{
        std::cout << "b1 is not actually D\n";
    }

    Base* b2 = new Derived;
    if(auto d = dynamic_cast<Derived*>(b2))
    {
        std::cout << "downcast from b2 to d successful\n";
        d->name(); // safe to call
    }

    delete b1;
    delete b2;

    Base * baseptr = new Derived();
    auto what = dynamic_cast<void*>(baseptr);
    // так нельзя
    // auto dptr = dynamic_cast<Derived*>(what);
    // только так
    auto dptr = reinterpret_cast<Derived*>(what);
    dptr->name();

    // так нельзя
    //Base * baseptrFromVoid = dynamic_cast<Base*>(what);
    return 0;
}
