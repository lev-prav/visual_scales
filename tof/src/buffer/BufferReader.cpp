#include "../../include/buffer/BufferReader.h"

std::optional <Image> BufferReader::get_data() {
    return buffer_.read(position_);
}

bool BufferReader::move_forward() {
    if (position_ >= buffer_.length()) {
        return false;
    }
    position_++;
    return true;
}

bool BufferReader::move_back() {
    if (position_ > 0) {
        position_--;
        return true;
    }
    return false;
}
