//
// Created by lev on 29.09.22.
//

#ifndef TOF_TOFSENSOR_H
#define TOF_TOFSENSOR_H
#pragma once

#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>
#include <stdexcept>
#include "ArenaApi.h"
#include "SaveApi.h"
#include "AcquisitionThread.h"
#include "ToFDevice.h"

namespace  ToF {
    class ToFSensor {
    public:
        explicit ToFSensor(std::shared_ptr<Buffer> buffer_ptr) : buffer_(std::move(buffer_ptr)) {}

        int connect();

        int run();
        int stop();

        int setDevice(const std::string& serial);

        inline auto countAvailableDevices() {
            return systemDevices.size();
        }

        inline auto countStreamingDevices() {
            return devices.size();
        }

        ~ToFSensor();

    private:
        int startStream();
        int stopStream(int deviceNumber = 0);

        std::vector<Arena::DeviceInfo>::iterator getToFDevice(const std::string& serialNumber);

        Arena::ISystem *pSystem = nullptr;
        std::vector<Arena::DeviceInfo> systemDevices;
        std::vector<std::unique_ptr<AcquisitionThread>> acqThreads;
        std::vector<std::shared_ptr<ToFDevice>> devices;
        std::string serial_;

        std::shared_ptr<Buffer> buffer_;

        std::thread acquisition_thread;
    };
};

#endif //TOF_TOFSENSOR_H
