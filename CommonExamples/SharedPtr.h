#include <iostream>
#include <atomic>
#include <memory>

template<typename T>
void defaultDel(T *ptr){
    delete ptr;
}

template<typename T>
struct ControlBlock{
    ControlBlock():_refCount(0){

    }

    ControlBlock(const int v):_refCount(v){

    }
    
    void deleter(T *ptr){
        delete ptr;
    }

    std::atomic<int> _refCount;
};

template<typename T>
class SharedPtr{
public:
    SharedPtr(): _ptr(nullptr), _ctr_block(nullptr){

    }

    SharedPtr(T *ptr): _ptr(ptr), _ctr_block(new ControlBlock<T>(1)) {

    }

    SharedPtr(const SharedPtr& r): _ptr(r._ptr), _ctr_block(r._ctr_block){
        ++(_ctr_block->_refCount);
    }

    SharedPtr& operator=(const SharedPtr &r ) noexcept{
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
        _ctr_block = new ControlBlock<T>(1);
    }



    ~SharedPtr(){
        --(_ctr_block->_refCount);
        check_and_del();
    }

private:
    void check_and_del(){
        if(_ctr_block->_refCount != 0)
            return;
        _ctr_block->deleter(_ptr);
        delete _ctr_block;
    }
private:
    T *_ptr;
    ControlBlock<T> *_ctr_block;
};
