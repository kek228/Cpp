#include <cstdint>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

// static_cast < new_type > ( expression )
// Что можно конвертить:
// 1. Если существует неявное преобразование из expression в new_type
// (конструктор или оператор преобрахования типа)
// то static_cast вернет временный объект new_type Temp(expression);
// потому то это и работает с преобразованием их float в int
// 2. Можно делать приведение типа от Base к Derived. НО только для невиртуальных Base.
// Можно и D кастить в B.
// Никаких проверок в рантайме не происходит
// 3. Можно делать static_cast<new_type &&> из любой ссылки. По сути move-ать
// 4. Можно кастовать void* к типу
// 5. Enum class к целому или float
// 6. Числа к Enum class

// Замечания:
// 1. ГЕНЕРИРУЕТ код
// 2. Небезапасно для виртуальных классов


struct B {
    int m = 0;
    void hello() const {
        std::cout << "Hello world, this is B!\n";
    }

    void incrM(int incr){
        m += incr;
    }
};
struct D : B {
    void hello() const {
        std::cout << "Hello world, this is D!\n";
    }
};

enum class E { ONE = 1, TWO, THREE };
enum EU { ONE = 1, TWO, THREE };

int main()
{
    // 1: initializing conversion
    int n = static_cast<int>(3.14);
    std::cout << "n = " << n << '\n';
    // есть преобразование int в vector<T>, тк есть конструктор
    std::vector<int> v = static_cast<std::vector<int>>(10);
    std::cout << "v.size() = " << v.size() << '\n';

    // 2: static downcast
    D d;
    B& br = d; // upcast via implicit conversion
    br.hello();
    D& another_d = static_cast<D&>(br); // downcast
    another_d.hello();
    // Указывет реально на D, а hello() вызовется для B
    B * bptr = static_cast<B*>(&d);
    bptr->hello();

    // 3: move руками
    std::vector<int> v2 = static_cast<std::vector<int>&&>(v);
    std::cout << "after move, v.size() = " << v.size() << '\n';

    // аналог (void) чтобы не ругался компилятор на warning
    static_cast<void>(v2.size());

    // 4. inverse of implicit conversion
    void* nv = &n;
    int* ni = static_cast<int*>(nv);
    std::cout << "*ni = " << *ni << '\n';
    // Взяли и скастовали из void
    auto wrongBptr = static_cast<B*>(nv);
    // отработает
    wrongBptr->hello();
    wrongBptr->incrM(100500);
    cout<<"wrongBptr->m == "<<wrongBptr->m<<endl; // 100503 (int m лег на int n)

    // 6. array-to-pointer followed by upcast
    D a[10];
    B* dp = static_cast<B*>(a);

    // 7. scoped enum to int or float
    E e = E::ONE;
    int one = static_cast<int>(e);
    std::cout << one << '\n';

    // 8. int to enum, enum to another enum
    E e2 = static_cast<E>(one);
    EU eu = static_cast<EU>(e2);

    // 9. pointer to member upcast
    int D::*pm = &D::m;
    std::cout << br.*static_cast<int B::*>(pm) << '\n';

    // 10. void* to any type
    void* voidp = &e;
    std::vector<int>* p = static_cast<std::vector<int>*>(voidp);
}