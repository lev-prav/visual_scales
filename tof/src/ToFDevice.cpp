#pragma once

#include "../../include/ToFDevice.h"


void ToF::ToFDevice::save() {
    int bits_per_pixel = pImage_->GetBitsPerPixel();
    int bytes_per_pixel = bits_per_pixel / 8;

    int im_height = pImage_->GetHeight(),
        im_width = pImage_->GetWidth(),
        buff_size = im_height*im_width * bytes_per_pixel;


    unsigned char* data = new unsigned char [buff_size];
    memcpy(data, (unsigned char*)pImage_->GetData(), buff_size);

    //std::cout<<"Convert bits :"<<bits<<" : "<<pImage_->GetBitsPerPixel()<<"\n";
    buffer_->push_back(
            {
                .width = im_width,
                .height = im_height,
                .bits_per_pixel = bits_per_pixel,
                .data = std::shared_ptr<unsigned char[]>(data)
            });
}

void ToF::ToFDevice::prepareDevice() {
    Arena::SetNodeValue<GenICam::gcstring>(
            pDevice_->GetNodeMap(),
            "PixelFormat",
            PIXEL_FORMAT_TRANSFORM);

    // enable stream auto negotiate packet size
    Arena::SetNodeValue<bool>(pDevice_->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);
    // enable stream packet resend
    Arena::SetNodeValue<bool>(pDevice_->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);

    // synchronization
    // source voltage
    Arena::SetNodeValue<GenICam::gcstring>(
            pDevice_->GetNodeMap(),
            "LineSelector",
            "Line4");

    Arena::SetNodeValue<GenICam::gcstring>(
            pDevice_->GetNodeMap(),
            "LineMode",
            "Output");

    Arena::SetNodeValue<bool>(
            pDevice_->GetNodeMap(),
            "VoltageExternalEnable",
            true);

    Arena::SetNodeValue<GenICam::gcstring>(
            pDevice_->GetNodeMap(),
            "LineActivationVoltage",
            "Low");

    // signal line
    Arena::SetNodeValue<GenICam::gcstring>(
            pDevice_->GetNodeMap(),
            "LineSelector",
            "Line1");

    Arena::SetNodeValue<GenICam::gcstring>(
            pDevice_->GetNodeMap(),
            "LineMode",
            "Output");

    Arena::SetNodeValue<bool>(
            pDevice_->GetNodeMap(),
            "LineInverter",
            true);

    Arena::SetNodeValue<GenICam::gcstring>(
            pDevice_->GetNodeMap(),
            "LineSource",
            "ExposureActive");


    Arena::SetNodeValue<bool>(
            pDevice_->GetNodeMap(),
            "AcquisitionFrameRateEnable",
            true);

    auto min_fps = Arena::GetNodeMin<double>(
            pDevice_->GetNodeMap(),
            "AcquisitionFrameRate");
    auto max_fps = Arena::GetNodeMax<double>(
            pDevice_->GetNodeMap(),
            "AcquisitionFrameRate");

    frameLimits_ = {
            .min = min_fps,
            .max = max_fps
    };

    Arena::SetNodeValue<double>(
            pDevice_->GetNodeMap(),
            "AcquisitionFrameRate",
            max_fps);

    fps_ = Arena::GetNodeValue<double>(
            pDevice_->GetNodeMap(),
            "AcquisitionFrameRate");

}

int ToF::ToFDevice::setFrameRate(double rate) {
    if (rate <  frameLimits_.min or rate > frameLimits_.max)
        return EXIT_FAILURE;

    Arena::SetNodeValue<double>(
            pDevice_->GetNodeMap(),
            "AcquisitionFrameRate",
            rate);
    fps_ = rate;
    return EXIT_SUCCESS;
}

const ToF::FrameLimits& ToF::ToFDevice::getFrameLimits() {
    return frameLimits_;
}

double ToF::ToFDevice::getFrameRate() {
    return fps_;
}
