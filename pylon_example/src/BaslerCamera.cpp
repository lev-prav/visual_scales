//
// Created by lev on 05.04.23.
//

#include "../include/BaslerCamera.h"
#include "../include/BaslerImage.h"

using namespace Pylon;
using namespace std;

BaslerCamera::BaslerCamera(cam_ptr camera, const std::shared_ptr<Buffer<BaslerImage>>& buffer, int index)
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

    camera_->AcquisitionMode.SetValue(Basler_UniversalCameraParams::AcquisitionMode_Continuous);
}

void BaslerCamera::start() {
    camera_->StartGrabbing();

}

void BaslerCamera::acquire() {

    std::cout<<"ACQUIRE ???\n";
    if( camera_->IsGrabbing()) {
        try {
            camera_->RetrieveResult(5000, ptrGrabResult);
        } catch (GenericException& ex){
            std::cout<<ex.what()<<"\n";
            succeeded_ = false;
            return ;
        }
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

    CPylonImage image;
    image.CopyImage(
            ptrGrabResult->GetBuffer(),
            ptrGrabResult->GetBufferSize(),
            ptrGrabResult->GetPixelType(),
            ptrGrabResult->GetWidth(),
            ptrGrabResult->GetHeight(),
            ptrGrabResult->GetPaddingX()
            );

    ptrGrabResult.Release();

    if (buffer_.expired()){
        std::cout<<"Buffer is NO MORE !\n";
        return ;
    }


    auto lock_buffer = buffer_.lock();
    lock_buffer->push_back({
                                   .camera_index = index_,
                                   .image = image
                           });
    std::cout<<"CAM BUF SIZE: "<<lock_buffer->size() <<"Length : " <<lock_buffer->length() <<" \n";
}

void BaslerCamera::stop() {
    camera_->StopGrabbing();

}

void BaslerCamera::clean() {
    //ptrGrabResult = CGrabResultPtr();
}

BaslerCamera::~BaslerCamera() {
    camera_->Close();
}

