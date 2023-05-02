//
// Created by lev on 04.04.23.
//

#include "../include/TwoCameras.h"

using namespace Pylon;
using namespace std;

const size_t c_maxCamerasToUse = 2;
const uint32_t c_countOfImagesToGrab = 10;


TwoCameras::TwoCameras(const std::string &save_dir, int buffer_size) {
    buffer_size_ = std::max(buffer_size,30);
    save_dir_ = save_dir;
    configure();

}

void TwoCameras::configure() {
    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();

    // Get the transport layer factory.
    CTlFactory &tlFactory = CTlFactory::GetInstance();

    // Get all attached devices and exit application if no device is found.
    DeviceInfoList_t devices;
    if (tlFactory.EnumerateDevices(devices) == 0) {
        throw RUNTIME_EXCEPTION("No camera present.");
    }

    // Create an array of instant cameras for the found devices and avoid exceeding a maximum number of devices.
     cameras = std::vector<cam_ptr>(min(devices.size(), c_maxCamerasToUse));

    std::cout << devices.size() << "\n";

    // Create and attach all Pylon Devices.
    for (size_t i = 0; i < cameras.size(); ++i) {
        auto dev = tlFactory.CreateDevice(devices[i]);

        cameras[i] = std::make_shared<cam>(dev);
        //cameras[i]->Attach(dev);

        try{
            cameras[i]->Open();
        } catch(const GenericException& ex){
            std::cout<<ex.what()<<"\n";
        }

        cameras[i]->LineSelector.SetValue(Basler_UniversalCameraParams::LineSelector_Line1);
        cameras[i]->TriggerMode.SetValue(Basler_UniversalCameraParams::TriggerMode_On);
        cameras[i]->LineMode.SetValue(Basler_UniversalCameraParams::LineMode_Input);

        auto buffer_ = std::make_shared<Buffer<BaslerImage>>(buffer_size_);
        auto saver_ = std::make_unique<BaslerSaver>(save_dir_, Buffer<BaslerImage>::get_reader(buffer_));
        auto basCam = std::make_shared<BaslerCamera>(cameras[i], buffer_, i);

        threads.emplace_back(std::make_unique<AcquisitionThread>(basCam));
        buffers_.emplace_back(buffer_);
        savers_.emplace_back(std::move(saver_));

        // Print the model name of the camera.
        cout << "Using device " << cameras[i]->GetDeviceInfo().GetModelName() << endl;
    }
}

void TwoCameras::run() {
    for(auto& thread : threads){
        thread->run();
    }
}

void TwoCameras::stop() {
    save_off();

    for(auto& thread : threads){
        thread->stop();
    }
}

TwoCameras::~TwoCameras() {
    // Releases all pylon resources.
    PylonTerminate();
}

void TwoCameras::save_on() {
    for(auto& saver : savers_){
        saver->run();
    }}

void TwoCameras::save_off() {
    for(auto& saver : savers_){
        saver->stop();
    }
}

