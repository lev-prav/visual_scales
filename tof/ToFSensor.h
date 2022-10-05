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


class ToFSensor {
public:
    ToFSensor() = default;

    int connect(){
        pSystem = Arena::OpenSystem();
        pSystem->UpdateDevices(100);
        systemDevices = pSystem->GetDevices();
        if (systemDevices.empty())
        {
            std::cout << "\nNo camera connected\nPress enter to complete\n";
            std::getchar();
            return EXIT_FAILURE;
        }
        std::cout<<"ToF sensor successfully Connected!\n";
        return EXIT_SUCCESS;
    }

    int startStream(int deviceNumber = 0) {
        if (pSystem == nullptr) return EXIT_FAILURE;
        if (deviceNumber >= systemDevices.size() or deviceNumber < 0)
            throw std::out_of_range("Device number out of range");


        //Arena::IDevice* pDevice = pSystem->CreateDevice(systemDevices[deviceNumber]);
        auto* device =  new ToFDevice(pSystem, pSystem->CreateDevice(systemDevices[deviceNumber]));
        std::cout<<"Device Preparation..\n";
        device->prepareDevice();

        devices.push_back(device);

        auto* acqThread = new AcquisitionThread(device);
        acqThread->run();
        acqThreads.push_back(acqThread);
        std::cout<<"Device number "<< deviceNumber << "starts streaming...\n";

    }

    int stopStream(int deviceNumber = 0){
        if (deviceNumber >= acqThreads.size() or deviceNumber < 0)
            throw std::out_of_range("Device number out of range");

        acqThreads[deviceNumber]->stop();
        std::cout<<"Device number "<< deviceNumber << "stop streaming...\n";
    }

    auto countAvailableDevices(){
        return systemDevices.size();
    }
    auto countStreamingDevices(){
        return devices.size();
    }

    ~ToFSensor(){
        Arena::CloseSystem(pSystem);
        for(AcquisitionThread* it : acqThreads){
            delete it;
        }
    }
private:
    Arena::ISystem* pSystem = nullptr;
    std::vector<Arena::DeviceInfo> systemDevices;
    std::vector<AcquisitionThread*> acqThreads;
    std::vector<ToFDevice*> devices;

};


#endif //TOF_TOFSENSOR_H
