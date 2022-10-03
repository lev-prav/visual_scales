//
// Created by lev on 29.09.22.
//

#ifndef TOF_TOFSENSOR_H
#define TOF_TOFSENSOR_H

#include <cstdlib>
#include <iostream>
#include <vector>
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
        if (systemDevices.size() == 0)
        {
            std::cout << "\nNo camera connected\nPress enter to complete\n";
            std::getchar();
            return 0;
        }
    }

    int startStream(int deviceNumber = 0){
        if (pSystem == nullptr) return EXIT_FAILURE;

        //Arena::IDevice* pDevice = pSystem->CreateDevice(systemDevices[deviceNumber]);
        ToFDevice* device = new ToFDevice(pSystem->CreateDevice(systemDevices[deviceNumber]));
        device->prepareDevice();

        devices.push_back(device); // ??

        acqThread = new AcquisitionThread(device); // ??
        acqThread->run();
    }

    int stopStream(){
        acqThread->stop();
    }

    ~ToFSensor(){
        delete pSystem;
        delete acqThread;

    }
private:
    Arena::ISystem* pSystem = nullptr;
    std::vector<Arena::DeviceInfo> systemDevices;
    AcquisitionThread* acqThread;
    std::vector<ToFSensor*> devices;

};


#endif //TOF_TOFSENSOR_H
