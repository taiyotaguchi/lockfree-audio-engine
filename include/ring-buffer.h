#pragma once
#include <optional>
#include <memory>
#include <cassert>

template <typename T>
class RingBuffer {
public:
    RingBuffer(size_t power_of_capacity) : capacity_(1 << power_of_capacity), arr_(std::make_unique<T[]>(capacity_))
    {
        assert(power_of_capacity >= 1);
    }

    bool try_push(T value) {
        size_t wrapped_ptr = write & (capacity_ - 1);
        if (full()) {
            return false;
        } else {
            arr_[wrapped_ptr] = value;
            write++;
            return true;
        }
    }

    std::optional<T> try_pop() {
        size_t wrapped_ptr = read & (capacity_ - 1);
        if (empty()) {
            return std::nullopt;
        } else {
            T output = arr_[wrapped_ptr];
            read++;
            return output;
        }
    }

    bool empty() const {
        return (write == read);
    }
    
    bool full() const {
        return (write - read == capacity_);
    }

    size_t size() const {
        return write - read;
    }

    size_t capacity() const {
        return capacity_;
    }

private:
    size_t capacity_;
    std::unique_ptr<T[]> arr_; 
    size_t write = 0;
    size_t read = 0;
};