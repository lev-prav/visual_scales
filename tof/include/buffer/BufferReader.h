#pragma once
#include "../Image.h"
#include "Buffer.h"
#include <optional>

class Buffer;

class BufferReader{
public:
    BufferReader(Buffer& buffer) : buffer_(buffer) {}

    bool                    move_forward();
    bool                    move_back();
    std::optional<Image>    get_data();
private:
    unsigned int position_ = 0;
    Buffer& buffer_;
};