//
// Created by LENOVO on 30.09.2022.
//

#ifndef TOF_TOFDEVICE_H
#define TOF_TOFDEVICE_H

#include "ArenaApi.h"
#include "SaveApi.h"
#include "IDevice.h"
#include "ToFSaver.h"

#define PIXEL_FORMAT_TRANSFORM "Coord3D_C16"
#define PIXEL_FORMAT Coord3D_C16

class ToFDevice : public IDevice{
public:
    ToFDevice(Arena::ISystem* pSystem, Arena::IDevice* pDevice) :
    pSystem(pSystem),
    pDevice(pDevice),
    saver(new ToFSaver("tof_images")){}

    void start() override {
        pDevice->StartStream();
    }

    void acquire() override {
        this->pImage = pDevice->GetImage(2000);
    }

    void save() override {
        saver->save(pImage);
    }

    void clean() override {
        pDevice->RequeueBuffer(pImage);
    }

    void stop() override {
        pDevice->StopStream();
    }

    void prepareDevice(){
        Arena::SetNodeValue<GenICam::gcstring>(
                pDevice->GetNodeMap(),
                "PixelFormat",
                PIXEL_FORMAT_TRANSFORM);

        // enable stream auto negotiate packet size
        Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);
        // enable stream packet resend
        Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);
    }

    ~ToFDevice() {
        pSystem->DestroyDevice(pDevice);
        delete saver;
    }
private:
    Arena::ISystem* pSystem;
    Arena::IDevice* pDevice;
    Arena::IImage* pImage;
    ToFSaver* saver;
};

#endif //TOF_TOFDEVICE_H
