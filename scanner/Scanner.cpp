#include "Scanner.h"

int Scanner::run() {
    tof_->run();
    cameras_->run();

    return 0;
}

Scanner::Scanner(const std::string& tofSerialNumber, const std::string& output_dir, unsigned int buffer_size) {
    buffer_ = std::make_shared<Buffer<Image>>(buffer_size);

    tof_ = std::make_unique<ToF::ToFSensor>(buffer_);
    tof_->setDevice(tofSerialNumber);
    fpsController_ = std::make_shared<Scanner::FPSController>(this);

    saver_ = std::make_unique<ToF::ToFSaver>(output_dir, buffer_->get_reader());

    cameras_ = std::make_unique<TwoCameras>(output_dir, std::min(int(buffer_size), 19));
}

int Scanner::stop() {
    tof_->stop();
    cameras_->stop();
    saver_->stop();

    return 0;
}

std::shared_ptr<BufferReader<Image>> Scanner::get_buffer_reader() const{
    return buffer_->get_reader();
}


std::function<void(bool)> Scanner::get_listener() const{

    return [this](bool activate_) {
        if (activate_){
            activate();
        }else{
            deactivate();
        }
    };
}

void Scanner::activate() const {
    saver_->run();
    cameras_->save_on();
}

void Scanner::deactivate() const{
    saver_->stop();
    cameras_->save_off();
}

std::shared_ptr<Scanner::FPSController> Scanner::get_fps_controller() {
    return fpsController_;
}


