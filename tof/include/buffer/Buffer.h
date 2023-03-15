#pragma once
#include "../Image.h"
#include "BufferReader.h"
#include <deque>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <vector>

class BufferReader;

class Buffer {
public:
    explicit Buffer(unsigned int length = 60) : length_(length){}

    int push_back(const Image& image);
    std::optional<Image> read(unsigned int position);
    //add_reader(BufferReader reader);
    std::shared_ptr<BufferReader> get_reader();
    unsigned int length();

private:
    void notify_readers();

    unsigned int length_ = 0;
    std::deque<Image> buffer_;
    std::shared_mutex mutex_;

    std::vector<std::shared_ptr<BufferReader>> readers_;
};

