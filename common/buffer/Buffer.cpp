//#include "buffer/Buffer.h"
//
//int Buffer::push_back(const T &image) {
//    std::lock_guard lock_w(write_mutex_);
//    std::unique_lock lock(read_mutex_);
//
//    if (buffer_.size() > length_){
//        buffer_.pop_front();
//        notify_readers();
//    }
//    buffer_.push_back(image);
//
//    return 0;
//}
//
//std::optional<T>
//Buffer::read(unsigned int position) {
//    if (position >= buffer_.size()){
//        return {};
//    }
//
//    std::shared_lock lock(read_mutex_);
//    return buffer_[position];
//}
//
//void Buffer::notify_readers() {
//    for(auto& reader : readers_){
//        reader->move_back();
//    }
//}
//
//unsigned int Buffer::length() {
//    return length_;
//}
//
//std::shared_ptr<BufferReader> Buffer::get_reader() {
//    auto* raw_reader = new BufferReader(*this);
//
//    std::shared_ptr<BufferReader> reader(raw_reader);
//    readers_.push_back(reader);
//    return reader;
//}
