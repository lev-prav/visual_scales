#pragma once
#include "Image.h"
#include "Buffer.h"
#include <optional>

template <class T>
class Buffer;

template <class T>
class BufferReader{
public:
    BufferReader(Buffer<T>& buffer) : buffer_(buffer) {}

    bool                    move_forward(){
        if (position_ >= buffer_.length()) {
            return false;
        }
        position_++;
        return true;
    }
    bool                    move_back(){
        if (position_ > 0) {
            position_--;
            return true;
        }
        return false;
    }
    std::optional<T>    get_data(){
        return buffer_.read(position_);
    }
private:
    unsigned int position_ = 0;
    Buffer<T>& buffer_;
};