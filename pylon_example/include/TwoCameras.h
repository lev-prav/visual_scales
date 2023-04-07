#ifndef VISUAL_SCALES_TWOCAMERAS_H
#define VISUAL_SCALES_TWOCAMERAS_H

#include <vector>
#include <thread>
#include "BaslerCamera.h"
#include "AcquisitionThread.h"
#include "BaslerSaver.h"


class TwoCameras {
public:
    explicit TwoCameras(const std::string& save_dir, int buffer_size);
    void run();
    void stop();

    void save_on();
    void save_off();

    ~TwoCameras();
private:
    void configure();
    std::unique_ptr<BaslerSaver> saver_;
    std::shared_ptr<Buffer<BaslerImage>> buffer_;
    std::vector<cam_ptr> cameras;
    std::vector<std::unique_ptr<AcquisitionThread>> threads;
};


#endif //VISUAL_SCALES_TWOCAMERAS_H
