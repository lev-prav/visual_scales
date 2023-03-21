#pragma once

#include "../../include/ToFDevice.h"


void ToF::ToFDevice::save() {
    int bits_per_pixel = pImage_->GetBitsPerPixel();
    int bytes_per_pixel = bits_per_pixel / 8;

    int im_height = pImage_->GetHeight(),
        im_width = pImage_->GetWidth(),
        buff_size = im_height*im_width * bytes_per_pixel;


    unsigned char* data = new unsigned char [buff_size];

    auto size = pImage_->GetSizeOfBuffer();
    memcpy(data, (unsigned char*)pImage_->GetData(), buff_size);

    auto* rgb_image = Arena::ImageFactory::Convert(
            pImage_,
            Mono8);

    auto bits = rgb_image->GetBitsPerPixel();
    std::cout<<"Convert bits :"<<bits<<" : "<<pImage_->GetBitsPerPixel()<<"\n";
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

}
