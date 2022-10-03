//
// Created by lev on 29.09.22.
//

#ifndef TOF_TOFSENSOR_H
#define TOF_TOFSENSOR_H

\#include "ArenaApi.h"

#include <cstdlib>
#include <iostream>
#include "SaveApi.h"
#include "AcquisitionThread.h"

#define PIXEL_FORMAT_TRANSFORM "Coord_C16"
#define PIXEL_FORMAT Coord3D_C16

class ToFSensor {
public:
    ToFSensor() = default;

    int connect(){
        pSystem = Arena::OpenSystem();
        pSystem->UpdateDevices(100);
        devices = pSystem->GetDevices();
        if (devices.size() == 0)
        {
            std::cout << "\nNo camera connected\nPress enter to complete\n";
            std::getchar();
            return 0;
        }
    }

    int startStream(int deviceNumber = 0){
        if (pSystem == nullptr) return EXIT_FAILURE;

        Arena::IDevice* pDevice = pSystem->CreateDevice(devices[deviceNumber]);
        prepareDevice(pDevice);
        pDevice->StartStream();

        int n = 0;
        while(1) {
            Arena::IImage* pImage = pDevice->GetImage(2000);

            std::string fname("images/image_");
            fname += std::to_string(n) + std::string(".tiff");
            std::cout << "Saving image with fname: " << fname << "\n";
            saveImage(pImage, fname.c_str());
            n++;
            // clean up example
            pDevice->RequeueBuffer(pImage);
        }
    }

    int stopStream(){

    }
private:
    Arena::ISystem* pSystem = nullptr;
    std::vector<Arena::DeviceInfo> devices;
    AcquisitionThread<ToFSensor>* acThread;

    void prepareDevice(Arena::IDevice* pDevice){
        Arena::SetNodeValue<GenICam::gcstring>(
                pDevice->GetNodeMap(),
                "PixelFormat",
                PIXEL_FORMAT_TRANSFORM);

        // enable stream auto negotiate packet size
        Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);
        // enable stream packet resend
        Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);
    }

};


#endif //TOF_TOFSENSOR_H
