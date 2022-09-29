//
// Created by lev on 29.09.22.
//

#ifndef TOF_TOFSENSOR_H
#define TOF_TOFSENSOR_H

#include "ArenaApi.h"
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

    void saveImage(Arena::IImage* pImage, const char* filename){
        auto pConverted = Arena::ImageFactory::Convert(
                pImage,
                PIXEL_FORMAT);

        Save::ImageParams params(
                pConverted->GetWidth(),
                pConverted->GetHeight(),
                pConverted->GetBitsPerPixel());

        Save::ImageWriter writer(
                params,
                filename);

        // Save image
        //    Passing image data into the image writer using the cascading I/O
        //    operator (<<) triggers a save. Notice that the << operator accepts the
        //    image data as a constant unsigned 8-bit integer pointer (const
        //    uint8_t*) and the file name as a character string (const char*).

        writer << pConverted->GetData();
        // destroy converted image
        Arena::ImageFactory::Destroy(pConverted);
    }
};


#endif //TOF_TOFSENSOR_H
