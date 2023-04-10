//
// Created by lev on 05.04.23.
//

#ifndef VISUAL_SCALES_BASLERCAMERA_H
#define VISUAL_SCALES_BASLERCAMERA_H

#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

#include "IDevice.h"
#include "buffer/Buffer.h"
#include "BaslerImage.h"

using cam = Pylon::CBaslerUniversalInstantCamera;
using cam_ptr = std::shared_ptr<cam>;

class BaslerCamera  : public IDevice {
public:
    explicit BaslerCamera(cam_ptr camera, const std::shared_ptr<Buffer<BaslerImage>>& buffer, int index);

    void start() override;
    void acquire() override;
    void save() override;
    void clean() override;
    void stop() override;

    ~BaslerCamera();
private:
    int index_;
    unsigned long long images_grabbed_ = 0;
    bool succeeded_ = false;
    cam_ptr camera_;
    std::weak_ptr<Buffer<BaslerImage>> buffer_;
    Pylon::CGrabResultPtr ptrGrabResult;
};


#endif //VISUAL_SCALES_BASLERCAMERA_H
