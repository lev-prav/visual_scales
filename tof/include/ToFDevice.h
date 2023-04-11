//
// Created by LENOVO on 30.09.2022.
//

#ifndef TOF_TOFDEVICE_H
#define TOF_TOFDEVICE_H

#include "ArenaApi.h"
#include "SaveApi.h"
#include "../../common/IDevice.h"
#include "buffer/Buffer.h"

#define PIXEL_FORMAT_TRANSFORM "Coord3D_C16"
#define PIXEL_FORMAT Coord3D_C16


namespace ToF{
    struct FrameLimits{
        double min = 0;
        double max = 0;
    };

class ToFDevice : public IDevice {
public:
    ToFDevice(Arena::IDevice *pDevice, const std::shared_ptr<Buffer<Image>>& buffer):
    pDevice_(pDevice), buffer_(buffer){
        //saver = std::make_unique<ToFSaver>("/home/lev/data/4Cameras/visual_scales/data/");
    }

    void start() override {
        pDevice_->StartStream();
    }

    void acquire() override {
        try{
            pImage_ = pDevice_->GetImage(2000);
            images_grabbed_++;
        } catch (...){
        }

    }

    void save() override;

    void clean() override {
        pDevice_->RequeueBuffer(pImage_);
    }

    void stop() override {
        pDevice_->StopStream();
    }

    void prepareDevice();

    Arena::IDevice* getDevicePtr(){
        return pDevice_;
    }

    int setFrameRate(double rate);
    double getFrameRate();
    const FrameLimits& getFrameLimits();

    ~ToFDevice() override {
        std::cout<<"DESTROY DEVICE\n";
    }

private:
    double fps_;
    unsigned long long images_grabbed_ = 0;
    FrameLimits frameLimits_;

    Arena::IDevice* pDevice_;
    Arena::IImage* pImage_;
    std::shared_ptr<Buffer<Image>> buffer_;
};

}
#endif //TOF_TOFDEVICE_H
