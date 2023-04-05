#pragma once
#include "Image.h"
#include "BufferReader.h"
#include <deque>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <vector>

template <class T>
class BufferReader;

template <class T>
class Buffer {
public:
    explicit Buffer(unsigned int length = 60) : length_(length){}

    int push_back(const T& image){
        std::lock_guard lock_w(write_mutex_);
        std::unique_lock lock(read_mutex_);

        if (buffer_.size() > length_){
            buffer_.pop_front();
            notify_readers();
        }
        buffer_.push_back(image);

        return 0;
    };
    std::optional<T> read(unsigned int position){
        if (position >= buffer_.size()){
            return {};
        }

        std::shared_lock lock(read_mutex_);
        return buffer_[position];
    };
    std::shared_ptr<BufferReader<T>> get_reader(){
        auto* raw_reader = new BufferReader(*this);

        std::shared_ptr<BufferReader<T>> reader(raw_reader);
        readers_.push_back(reader);
        return reader;
    };
    unsigned int length(){
        return length_;
    };

private:
    void notify_readers(){
        for(auto& reader : readers_){
            reader->move_back();
        }
    };

    unsigned int length_ = 0;
    std::deque<T> buffer_;
    std::shared_mutex read_mutex_;
    std::mutex write_mutex_;

    std::vector<std::shared_ptr<BufferReader<T>>> readers_;
};
