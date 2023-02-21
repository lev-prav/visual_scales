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


namespace ToF{
class ToFDevice : public IDevice {
public:
    ToFDevice(Arena::IDevice *pDevice):
    pDevice(pDevice){
        saver = std::make_unique<ToFSaver>("/home/lev/data/4Cameras/visual_scales/data/");
    }

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

    void prepareDevice() {
        Arena::SetNodeValue<GenICam::gcstring>(
                pDevice->GetNodeMap(),
                "PixelFormat",
                PIXEL_FORMAT_TRANSFORM);

        // enable stream auto negotiate packet size
        Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);
        // enable stream packet resend
        Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);

        // synchronization
        // source voltage
        Arena::SetNodeValue<GenICam::gcstring>(
                pDevice->GetNodeMap(),
                "LineSelector",
                "Line4");

        Arena::SetNodeValue<GenICam::gcstring>(
                pDevice->GetNodeMap(),
                "LineMode",
                "Output");

        Arena::SetNodeValue<bool>(
                pDevice->GetNodeMap(),
                "VoltageExternalEnable",
                true);

        Arena::SetNodeValue<GenICam::gcstring>(
                pDevice->GetNodeMap(),
                "LineActivationVoltage",
                "Low");

        // signal line
        Arena::SetNodeValue<GenICam::gcstring>(
                pDevice->GetNodeMap(),
                "LineSelector",
                "Line1");

        Arena::SetNodeValue<GenICam::gcstring>(
                pDevice->GetNodeMap(),
                "LineMode",
                "Output");

        Arena::SetNodeValue<bool>(
                pDevice->GetNodeMap(),
                "LineInverter",
                true);

        Arena::SetNodeValue<GenICam::gcstring>(
                pDevice->GetNodeMap(),
                "LineSource",
                "ExposureActive");

    }
    Arena::IDevice* getDevicePtr(){
        return pDevice;
    }

    ~ToFDevice() override {
        std::cout<<"DESTROY DEVICE\n";
    }

private:
    Arena::IDevice* pDevice;
    Arena::IImage* pImage;
    std::unique_ptr<ToFSaver> saver;
};

}
#endif //TOF_TOFDEVICE_H
