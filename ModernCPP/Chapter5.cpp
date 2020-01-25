#include <iostream>
#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#include <boost/stacktrace.hpp>
#include <boost/type_index.hpp>
using namespace std;
using boost::typeindex::type_id_with_cvr;

// ITEM 23
// В этом примере input НЕ СМУВАЕТСЯ в value, а СКОПИРУЕТСЯ
// Почему: потому что std::move(input) вернет const string&&.
// а const T&& байндятся к функциям f(const T&) -> будет вызван конструктор копирования
// вывод: don’t declare objects const if you want to be able to move from them

class Annotation {
public:
    explicit Annotation(const std::string input):
        value(std::move(input))
    {}
private:
    std::string value;
};

// Пример std::forward
// std::forward кастует && если реально был передан &&.
void process(const string& lvalArg){}
void process(string&& rvalArg){}
template<typename T>
void logAndProcess(T&& param)
{
    auto now = // get current time
            std::chrono::system_clock::now();
    process(std::forward<T>(param));
}

// Пример заменяемости move на forward
class Widget {
    string s;
    static int moveCtorCalls;
public:
    Widget() = default;
    Widget(Widget &&rhs) : s(std::move(rhs.s)) // move на аргумент надо делать РУКАМИ
            { ++moveCtorCalls; }

//    работает так же как с move
//    Widget(Widget &&rhs) : s(forward<string>(rhs.s))
//    { ++moveCtorCalls; }

};

void item23(){
    cout<<"ITEM 23"<<endl;
    // пример когда move не смувал
    string test = "test";
    Annotation anat(test);
    int a = 1;
}

// ITEM24. Как отличить rval ref от uni ref ( forwarding reference в стандарте )
// Если происходит вывод типа, и синтаксис имеет вид T&& или auto &&, то это un ref
// Примеры
void f(Widget&& param){
    cout<<"f(Widget&& param)"<<endl;
} // rvalue reference

template<typename T>
void f(std::vector<T>&& param){ // rvalue reference
    cout<<"f(std::vector<T>&& param)"<<endl;
}

template<typename T>
void f(T&& param){ // un reference
    cout<<endl<<"f(T&& param)"<<endl;
    cout << "PARAM TYPE="<<type_id_with_cvr<decltype(param)>().pretty_name() <<endl<<endl;

}

// Достаточно ПРОСТО добавить const и это уже не un ref
template<typename T>
void f(const T&& param){
    cout<<"f(const T&& param)"<<endl;
    cout << "PARAM TYPE="<<type_id_with_cvr<decltype(param)>().pretty_name() <<endl<<endl;
}


void item24(){
    cout<<"ITEM 24"<<endl;
    Widget&& var1 = Widget(); // rvalue reference
    auto&& var2 = var1; // un reference
    const int a = 1;
    f(a); // f(T&& param) PARAM TYPE=int const&
    f(10); // f(T&& param) PARAM TYPE=int&&
    const int &&b  = 1;// int const&&
    // b rval,
    cout << "b type="<<type_id_with_cvr<decltype(b)>().pretty_name() << std::endl;
    // У параметра есть имя, значит уже не rval
    f(b); // f(T&& param) PARAM TYPE=int const&
    // только после move-a будет вызвана перегрузка для const T&& param
    f(move(b)); // f(const T&& param) PARAM TYPE=int const&&
    // move НЕ ВЛИЯЕТ на cv-
}

// Не надо move-ать un ref, их надо форвардить.
// Когда надо move-ать  возвращаемое значение функции?
// 1. Когда надо
class Matrix{
    int val;
public:
    Matrix(){
        val = 1;
        // cout<<"Matrix()"<<endl;
    }

    Matrix(const Matrix&){
        cout<<"Matrix(const Matrix&)"<<endl;
    }

    Matrix(Matrix&&) noexcept {
        cout<<"Matrix(Matrix&&)"<<endl;
    }

    friend Matrix& operator+=(Matrix& left, const Matrix& right);

    friend Matrix operator+(Matrix&& lhs, const Matrix& rhs);

    friend Matrix operator-(Matrix&& lhs, const Matrix& rhs);
};

Matrix& operator+=(Matrix& left, const Matrix& right) {
    left.val += right.val;
    return left;
}

Matrix operator+(Matrix&& lhs, const Matrix& rhs)
{
    lhs += rhs;
    // Если не мувать в этой строке, вызовется КОНСТРУКТОР КОПИРОВАНИЯ Matrix(const Matrix&)
    // Если мувать: Matrix(Matrix&&)
    //  Почему. В теле функции lhs это lvalue, так что если мы хотим вызвать
    //  конструктор перемещения надо явно мувать
    return move(lhs);
}

Matrix operator-(Matrix&& lhs, const Matrix& rhs)
{
    lhs += rhs;
    return lhs;
}

// Локальные объекты, которые возвращаются по значению move-aть не надо
void item25(){
    cout<<"ITEM 25"<<endl;
    Matrix a;
    Matrix b;
    auto c = move(a) + b; // Matrix(Matrix&&)
    auto d = move(c) - b; // Matrix(const Matrix&)
}
int main(){
    item23();
    item24();
    item25();
}
