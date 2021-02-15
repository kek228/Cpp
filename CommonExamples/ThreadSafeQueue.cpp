#include "ThreadSafeQueue.h"
#include <vector>
#include <thread>
#include <iostream>
const int N = 10000;
int main() {
    ThreadSafeQueue<std::vector<int>> q;
    auto writer = [&q]() {
        for (int i = 0; i < N; ++i) {
            if (i % 2)
                q.emplace(i, i);
            else {
                std::vector<int> data(i, i);
                for (int j = 0; j < i; ++j)
                    data[j] = j;
                q.push(data);
            }
        }
    };

    auto reader = [&q]() {
        for (int i = 0; i < N; ++i) {
            if (i % 2) {
                std::unique_ptr<std::vector<int>> val = q.pop();
                if (!val)
                    continue;
                for (auto &v: *val)
                    ++v;
            } else {
                std::vector<int> val;
                q.pop(val);
                for (auto &v: val)
                    ++v;
            }
        }
    };

    std::vector<std::thread> threads;
    for(int i = 0; i < 100; ++i) {
        if (i % 2) {
            threads.emplace_back(writer);
        }else{
            threads.emplace_back(reader);
        }
    }
    for(auto &t: threads)
            t.join();
    return 0;
}