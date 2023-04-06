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

    std::cout<<"ACQUIRE ???\n";
    if( camera_->IsGrabbing()) {
        std::cout<<"ACQUIRE -\n";
        camera_->RetrieveResult(3000, ptrGrabResult);
        std::cout<<"ACQUIRE + \n";
        // Image grabbed successfully?
        succeeded_ = ptrGrabResult->GrabSucceeded();
        if (not succeeded_) {
            cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " "
                 << ptrGrabResult->GetErrorDescription() << endl;
        }
    }
}

void BaslerCamera::save() {
    if (not succeeded_)
        return;

    BaslerImage basImg;
    basImg.camera_index = index_;
    basImg.ptrGrabResult = ptrGrabResult;
    std::cout<<"Push baby push\n";
    buffer_->push_back(basImg);
    std::cout<<"CAM BUF SIZE: "<<buffer_->size() <<"Length : " <<buffer_->length() <<" \n";
}

void BaslerCamera::stop() {
    camera_->StopGrabbing();
}

void BaslerCamera::clean() {
    //ptrGrabResult = CGrabResultPtr();
}

