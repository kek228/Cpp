#include <iostream>
#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#include <boost/stacktrace.hpp>
using namespace std;

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
    Widget(Widget &&rhs) : s(std::move(rhs.s)) // move на аргумент надо делать РУКАМИ
            { ++moveCtorCalls; }

//    работает так же как с move
//    Widget(Widget &&rhs) : s(forward<string>(rhs.s))
//    { ++moveCtorCalls; }

};

void item23(){
    // пример когда move не смувал
    string test = "test";
    Annotation anat(test);
    int a = 1;
}
int main(){
    item23();
}
