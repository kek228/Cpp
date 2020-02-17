#include <iostream>
#include <vector>

using namespace std;

// const_cast < new_type > ( expression )
// Снимает cv-спецификатор c expression. ЕДИНСТВЕННЫЙ не считая c-style cast
// Что можно конвертить (добавлять/убирать const):
// 1. Указатели на один тип
// 2. lval к lval или rval, rval к rval. Все одних типов

// Замечания:
// не работает с указателями на функции
// не генерирует код
// САМОЕ ГЛАВНОЕ:
// через const_cast можно создать неконстантный указатель на КОНСТАНТУ
// модификация такого объекта через этот указатель: UNDEFINED BEHAVIOR

struct type {
    int i;

    type() : i(3) {}

    void f(int v) const {
        // все норм если конкрентный объект не константный
        const_cast<type *>(this)->i = v;
    }
};

int main() {
    int i = 3;                 // i is not declared const
    const int &rci = i;
    const_cast<int &>(rci) = 4; // OK: modifies i
    std::cout << "i = " << i << '\n';

    type t; // if this was const type t, then t.f(4) would be undefined behavior
    t.f(4);
    std::cout << "type::i = " << t.i << '\n';

    const int j = 3; // j is declared const
    int *pj = const_cast<int *>(&j);
    // ТАК НЕЛЬЗЯ UNDEFINED BEHAVIOR
    *pj = 100500;
    cout << "j after UNDEFINED modifying: " << j << endl;

    void (type::* pmf)(int) const = &type::f; // pointer to member function

    // Const_cast to 'void (type::*)(int)', which is not a reference, pointer-to-object, or pointer-to-data-member
    // с указателем на функцию НЕ КОМПИЛИТСЯ
    // const_cast<void(type::*)(int)>(pmf);
    const type t1;
    t1.f(100500);
    // Вообще это тоже UNDEFINED BEHAVIOR
    cout << "t1.i == " << t1.i << endl;
    return 0;
}