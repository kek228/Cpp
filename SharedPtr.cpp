#include <iostream>
#include <atomic>
#include <memory>

template<typename T, typename DEL>
struct control_block{
    control_block():_refCount(0){

    }

    control_block(const int v):_refCount(v){

    }

    std::atomic<T> _refCount;
    DEL deleter;
};

template<typename T, typename DEL>
class shared_ptr{
public:
    shared_ptr():_ptr(nullptr), _ctr_block(nullptr){

    }

    shared_ptr(T *ptr):_ptr(ptr), _ctr_block(new control_block<(1)){

    }

    shared_ptr(const shared_ptr& r):_ptr(r._ptr), _ctr_block(r._ctr_block){
        ++(_ctr_block->_refCount);
    }

    shared_ptr& operator=( const shared_ptr &r ) noexcept{
        if(this == &r)
            return *this;
        --(_ctr_block->_refCount);
        check_and_del();
        _ptr = r._ptr;
        _ctr_block = r._ctr_block;
        return *this;
    }

    void reset(int *ptr){
        --(_ctr_block->_refCount);
        check_and_del();
        _ptr = ptr;
        _ctr_block = new control_block(1);
    }



    ~shared_ptr(){
        --(_ctr_block->_refCount);
        check_and_del();
    }

private:
    void check_and_del(){
        if(_ctr_block->_refCount != 0)
            return;
        delete _ptr;
        delete _ctr_block;
    }
private:
    T *_ptr;
    control_block<T, DEL> *_ctr_block;
};

int main(){
}