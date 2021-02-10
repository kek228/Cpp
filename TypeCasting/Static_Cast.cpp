#include <cstdint>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

// static_cast < new_type > ( expression )
// Что можно конвертить:
// 1. Если существует неявное преобразование из expression в new_type
// (конструктор или оператор преоразования типа)
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

struct virtual_B {
    vector<int> v;
    int m = 0;
    virtual_B() {
        v = vector<int>(10, 0);
        v[5]= 12345;
    }

    virtual void hello() const {
        std::cout << "Hello world, this is virtual_B!" << v[5]<<endl;
    }

    void incrM(int incr){
        m += incr;
    }
};

struct virtual_D: public virtual_B {
    int k = 100500;
    vector<int> v;
    int m = 0;
    virtual_D() {
        v = vector<int>(10, 0);
        v[5]= 12345;
    }
    void hello() const override {
        std::cout << "Hello world, this is virtual_D! "<<k<<endl;
    }

    void incrM(int incr){
        m += incr;
    }
};


enum class E { ONE = 1, TWO, THREE };
enum EU { ONE = 1, TWO, THREE };

int main()
{
    // 1: initializing conversion
    int n = static_cast<int>(3.14);
    std::cout << "1: initializing conversion " << n << '\n';
    std::cout << "n = " << n << '\n';
    // есть преобразование int в vector<T>, тк есть конструктор
    std::vector<int> v = static_cast<std::vector<int>>(10);
    std::cout << "v.size() = " << v.size() << '\n';

    // 2: static downcast
    std::cout << "2: static downcast" << n << '\n';
    D d;
    B& br = d; // upcast via implicit conversion
    br.hello();
    D& another_d = static_cast<D&>(br); // downcast
    another_d.hello();
    // Указывет реально на D, а hello() вызовется для B
    B * bptr = static_cast<B*>(&d);
    bptr->hello();

    std::cout << "unsafe downcast" << n << '\n';
    virtual_D virt_d;
    virtual_B& virt_b = virt_d; // upcast via implicit conversion
    virt_b.hello(); // Hello world, this is virtual_D! (cause of virtual)
//    Clang-Tidy: Do not use static_cast to downcast from a base to a derived class;
//    use dynamic_cast instead
    virtual_D& another_virt_d = static_cast<virtual_D&>(virt_b); // downcast
    another_virt_d.hello(); // virtual_D passed cause virt_b is a real virtual_D
    virtual_B false_virt_d;
    virtual_D& failed_virt_d = static_cast<virtual_D&>(false_virt_d);
    failed_virt_d.hello(); // Hello world, this is virtual_B!
    virtual_D *failed_virt_d_ptr = dynamic_cast<virtual_D *>(&false_virt_d); // nullptr
    cout<<"failed_virt_d_ptr addr = "<<failed_virt_d_ptr<<endl;

    // 3: move руками
    std::cout << "3: move" << n << '\n';
    std::vector<int> v2 = static_cast<std::vector<int>&&>(v);
    std::cout << "after move, v.size() = " << v.size() << '\n';

    // аналог (void) чтобы не ругался компилятор на warning
    static_cast<void>(v2.size());

    // 4. inverse of implicit conversion
    std::cout << "4. inverse of implicit conversion" << n << '\n';
    void* nv = &n;
    int* ni = static_cast<int*>(nv);
    std::cout << "*ni = " << *ni << '\n';
    // Взяли и скастовали из void
    auto wrongBptr = static_cast<B*>(nv);
    // отработает
    wrongBptr->hello();
    wrongBptr->incrM(100500);
    cout<<"wrongBptr->m == "<<wrongBptr->m<<endl; // 100503 (int m лег на int n в стеке)

    // 5. array-to-pointer followed by upcast
    std::cout << "5. array-to-pointer followed by upcastn" << n << '\n';
    D a[10];
    B* dp = static_cast<B*>(a);

    // 6. scoped enum to int or float
    std::cout << "6. scoped enum to int or float" << n << '\n';
    E e = E::ONE;
    int one = static_cast<int>(e);
    std::cout << one << '\n';

    // 7. int to enum, enum to another enum
    std::cout << "7. int to enum, enum to another enum" << n << '\n';
    E e2 = static_cast<E>(one);
    EU eu = static_cast<EU>(e2);

    // 8. pointer to member upcast
    std::cout << "8. pointer to member upcast" << n << '\n';
    int D::*pm = &D::m;
    std::cout << br.*static_cast<int B::*>(pm) << '\n';

    // 9. void* to any type
    std::cout << "9. void* to any type" << n << '\n';
    void* voidp = &e; // E e = E::ONE;
    std::vector<int>* p = static_cast<std::vector<int>*>(voidp);
}