//
// Created by lev on 05.04.23.
//

#include "../include/BaslerCamera.h"
#include "../include/BaslerImage.h"

using namespace Pylon;
using namespace std;

BaslerCamera::BaslerCamera(cam_ptr camera, std::shared_ptr<Buffer<BaslerImage>> buffer, int index)
: camera_(std::move(camera)), index_(index), buffer_(buffer)
{
    try{
        camera_->Open();
    } catch(const GenericException& ex){
        std::cout<<ex.what()<<"\n";
    }

    camera_->LineSelector.SetValue(Basler_UniversalCameraParams::LineSelector_Line1);
    camera_->TriggerMode.SetValue(Basler_UniversalCameraParams::TriggerMode_On);
    camera_->LineMode.SetValue(Basler_UniversalCameraParams::LineMode_Input);
}

void BaslerCamera::start() {
    camera_->StartGrabbing();

}

void BaslerCamera::acquire() {

    if( camera_->IsGrabbing()) {
        camera_->RetrieveResult(INFINITE, ptrGrabResult);

        // Image grabbed successfully?
        if (ptrGrabResult->GrabSucceeded()) {

            intptr_t cameraContextValue = ptrGrabResult->GetCameraContext();
            counter_++;
            // Print the index and the model name of the camera.
/*            cout << "Camera " << index_ << ": " << camera_->GetDeviceInfo().GetModelName() << "\n"
                 << "GrabSucceeded: " << ptrGrabResult->GrabSucceeded() << "\n"
                 << "SizeX: " << ptrGrabResult->GetWidth() << "\n"
                 << "SizeY: " << ptrGrabResult->GetHeight() << "\n";
*/

        } else {
            cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " "
                 << ptrGrabResult->GetErrorDescription() << endl;
        }
    }
}

void BaslerCamera::save() {
    BaslerImage basImg;
    basImg.camera_index = index_;
    basImg.counter = counter_;
    basImg.ptrGrabResult = ptrGrabResult;

    buffer_->push_back(basImg);
}

void BaslerCamera::stop() {
    camera_->StopGrabbing();
}

void BaslerCamera::clean() {

}

