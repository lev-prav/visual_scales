//
// Created by lev on 29.09.22.
//

#ifndef TOF_TOFSENSOR_H
#define TOF_TOFSENSOR_H
#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "ArenaApi.h"
#include "SaveApi.h"
#include "AcquisitionThread.h"
#include "ToFDevice.h"

namespace  ToF {
    class ToFSensor {
    public:
        ToFSensor() = default;

        int connect();

        int startStream(const std::string& serialNumber);

        int stopStream(int deviceNumber = 0);

        inline auto countAvailableDevices() {
            return systemDevices.size();
        }

        inline auto countStreamingDevices() {
            return devices.size();
        }

        ~ToFSensor();

    private:
        std::vector<Arena::DeviceInfo>::iterator getToFDevice(const std::string& serialNumber);

        Arena::ISystem *pSystem = nullptr;
        std::vector<Arena::DeviceInfo> systemDevices;
        std::vector<std::unique_ptr<AcquisitionThread>> acqThreads;
        std::vector<std::shared_ptr<ToFDevice>> devices;

    };
};

#endif //TOF_TOFSENSOR_H
