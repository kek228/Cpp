#include <iostream>
#include <atomic>
#include <memory>
#include <functional>

template<typename T>
class DefaultDeleter{
public:
    void operator()(T *ptr){
        delete ptr;
    }
};

template<typename T>
struct ControlBlock {

    ControlBlock() : _refCount(1), deleter(DefaultDeleter<T>()) {

    }

    template<typename DEL>
    ControlBlock(DEL del) : _refCount(1), deleter(del) {

    }

    std::function<void(T*)> deleter;
    std::atomic<int> _refCount;
};

template<typename T>
class SharedPtr {
public:
    SharedPtr() : _ptr(nullptr), _ctr_block(nullptr) {

    }
    template<typename Y>
    SharedPtr(Y *ptr) : _ptr(ptr), _ctr_block(new ControlBlock<T>()) {

    }

    SharedPtr(const SharedPtr<T> &r) {
        _ptr = r._ptr;
        _ctr_block.store(r._ctr_block);
        ++(*_ctr_block)._refCount;
    }

    template<typename Y, typename DEL>
    SharedPtr(Y* ptr, DEL d): _ptr(ptr), _ctr_block(new ControlBlock<T>(d)){
    }

    SharedPtr &operator=(const SharedPtr &r) noexcept {
        if (this == &r)
            return *this;
        ++(_ctr_block->_refCount);
        _ptr = r._ptr;
        _ctr_block = r._ctr_block;
        return *this;
    }

    void reset(T *ptr) {
        check_and_del();
        _ptr = ptr;
        if(ptr)
            _ctr_block.store(new ControlBlock<T>());
    }


    ~SharedPtr() {
        check_and_del();
    }

private:
    void check_and_del() {
        if(!_ctr_block)
            return;
        const int prevVal = (*_ctr_block)._refCount.fetch_sub(1);
        if(prevVal != 1)
            return;
        (*_ctr_block).deleter(_ptr);
        auto toDelete = _ctr_block.exchange(nullptr);
        delete toDelete;
    }

private:
    std::shared_ptr<int> a;
    T* _ptr;
    std::atomic< ControlBlock<T>* >_ctr_block;
};
