#pragma once
#include "Image.h"
#include "Buffer.h"
#include <optional>

template <class T>
class Buffer;

template <class T>
class BufferReader{
    friend class Buffer<T> ;
public:

    bool move_forward(){

        if (buffer_.expired() or position_ >= buffer_.lock()->length()) {
            return false;
        }
        position_++;
        return true;
    }
    bool move_back(){
        if (position_ > 0) {
            position_--;
            return true;
        }
        return false;
    }
    std::optional<T>    get_data(){
        if (buffer_.expired())
            return {};

        return buffer_.lock()->read(position_);
    }
protected:
    explicit BufferReader(const std::shared_ptr<Buffer<T>>& buffer) : buffer_(buffer) {
    }
private:
    unsigned int position_ = 0;
    std::weak_ptr<Buffer<T>> buffer_;
};