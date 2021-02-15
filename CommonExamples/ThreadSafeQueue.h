#pragma once
#include <mutex>
#include <memory>
#include <queue>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() {}

    template<typename U>
    void push(U &&val) {
        std::unique_lock<std::mutex> _(_block);
        _q.emplace(std::forward<T>(val));
    }

    template<typename ...Args>
    void emplace(Args&& ... args) {
        std::unique_lock<std::mutex> _(_block);
        _q.emplace(std::forward<Args>(args)...);
    }

    std::unique_ptr<T> pop() {
        std::unique_lock<std::mutex> _(_block);
        if(_q.empty())
            return nullptr;
        auto res = std::make_unique<T>(std::move(_q.front()));
        _q.pop();
        return res;
    }

    void pop(T &res) {
        std::unique_lock<std::mutex> _(_block);
        if(_q.empty())
            return;
        res = std::move(_q.front());
        _q.pop();
    }

private:
    std::queue<T> _q;
    std::mutex _block;
};
