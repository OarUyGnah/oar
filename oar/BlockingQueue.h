#ifndef __OAR_BLOCKINGQUEUE_H__
#define __OAR_BLOCKINGQUEUE_H__

#include "Condition.h"

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>

namespace oar {

template <typename T, class Vec = std::deque<T>>
class BlockingQueue : Noncopyable {
public:
    BlockingQueue()
        : _mutex()
        , _cond(_mutex)
        , _queue()
    {
    }

    void push(const T& t)
    {
        MutexGuard mg(_mutex);
        _queue.push_back(t);
        _cond.notify();
    }

    void push(T&& t)
    {
        MutexGuard mg(_mutex);
        _queue.push_back(std::move(t));
        _cond.notify();
    }

    T take()
    {
        MutexGuard mg(_mutex);
        while (_queue.empty()) {
            _cond.wait();
        }
        assert(!_queue.empty());
        T t(std::move(_queue.front()));
        _queue.pop_front();
        return t;
    }

    size_t size() const
    {
        MutexGuard mg(_mutex);
        return _queue.size();
    }

    Vec takeall()
    {
        Vec vec;
        {
            MutexGuard mg(_mutex);
            vec = std::move(_queue);
            assert(vec.empty());
        }
    }

private:
    Mutex _mutex;
    Condition _cond;
    Vec _queue;
};

template <typename T>
class SafeQueue {
private:
    mutable std::mutex mut; // 1 互斥量必须是可变的
    std::deque<T> data_queue;
    std::condition_variable data_cond;

public:
    SafeQueue()
    {
    }
    SafeQueue(SafeQueue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};
}

#endif
