#include <cstdint>
#include <cassert>
#include <iostream>

using namespace std;
// reinterpret_cast < new_type > ( expression )

// Что можно конвертить:
// 1. Указатель к числу. Причем с потерями, можно указатель к 32 битному инту
// 2. Число/enum к указателю. Гарантируется сохранение значения адресса при обратном касте
// а вот если скастовать NULL nullptr не гарантируется
// 3. Любой указатель к любому указателю. Можно навесить < cv new_type>
// 4. Любую ссылку в любую l/r ссылку. Если все хорошо выровненно 3 и 4 пункт безопасны
// 5. Любой указатель на функцию в любой указатель на функцию. Правда будет UNDEFINED BEHAVIOR, если вызвать

int f() { return 42; }

int main(){
    int i = 7;

    // pointer to integer and back
    std::uintptr_t v1 = reinterpret_cast<std::uintptr_t>(&i); // static_cast is an error
    std::cout << "The value of &i is 0x" << std::hex << v1 << '\n';
    int* p1 = reinterpret_cast<int*>(v1);
    assert(p1 == &i);

    // pointer to function to another and back
    void(*fp1)() = reinterpret_cast<void(*)()>(f);
    // fp1(); undefined behavior
    int(*fp2)() = reinterpret_cast<int(*)()>(fp1);
    std::cout << std::dec << fp2() << '\n'; // safe

    // type aliasing through pointer
    char* p2 = reinterpret_cast<char*>(&i);
    if(p2[0] == '\x7')
        std::cout << "This system is little-endian\n";
    else
        std::cout << "This system is big-endian\n";

    // type aliasing through reference
    reinterpret_cast<unsigned int&>(i) = 42;
    std::cout << i << '\n';

    int num = 100500; // с добавлением cv
    auto ptr64 = reinterpret_cast<const int64_t*>(&num);
    // ptr to num val == -1705253371159410540 при чем значение постоянно меняется
    // при том что адресс тот же
    // Все потому что в старших 4 байтах лежит какое то говно
    cout<<"ptr to num val == " << *ptr64<<endl;
    cout<<"ADDR DIFF="<<reinterpret_cast<const int*>(ptr64) - &num;


    return 0;
}