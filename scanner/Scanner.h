#pragma once
#include "ToFSensor.h"
#include "ToFSaver.h"
#include "buffer/Buffer.h"
#include "../pylon_example/include/TwoCameras.h"

class Scanner {
public:
    class FPSController {
    public:
        FPSController(Scanner* scanner) : scanner_(scanner){}

        inline const ToF::FrameLimits& getFrameLimits(int deviceIndx = 0) const {
            if (scanner_->tof_->countStreamingDevices() > deviceIndx)
                return scanner_->tof_->getFrameLimits();
            std::cout<<scanner_->tof_->countStreamingDevices()<<"\n";
            return defaultLimits;
        }
        inline double getFrameRate(int deviceIndx = 0) const {
            if (scanner_->tof_->countStreamingDevices() > deviceIndx)
                return scanner_->tof_->getFrameRate();
            std::cout<<scanner_->tof_->countStreamingDevices()<<" GET \n";

            return 30.0;
        }
        inline bool setFrameRate(double rate, int deviceIndx = 0) const {
            if (scanner_->tof_->countStreamingDevices() > deviceIndx)
                return scanner_->tof_->setFrameRate(rate);
            std::cout<<scanner_->tof_->countStreamingDevices()<<" SET \n";

            return false;
        }
    private:
        Scanner* scanner_;
        ToF::FrameLimits defaultLimits {
            .min = 1.0,
            .max = 30.0
        };
    };

    explicit Scanner(const std::string& tofSerialNumber = "214100232",
                     const std::string& output_dir = "/home/lev/data/4Cameras/visual_scales/test/",
                     unsigned int buffer_size = 30);

    int run();
    int stop();

    std::function<void(bool)> get_listener() const;
    std::shared_ptr<BufferReader<Image>> get_buffer_reader() const;
    std::shared_ptr<FPSController> get_fps_controller();

private:
    void activate() const;
    void deactivate() const;

    std::atomic<bool> work = true;
    std::unique_ptr<ToF::ToFSensor> tof_;
    std::unique_ptr<ToF::ToFSaver> saver_;
    std::unique_ptr<TwoCameras> cameras_;
    std::string output_dir;
    std::shared_ptr<Buffer<Image>> buffer_;
    std::shared_ptr<FPSController> fpsController_;
};

