//
// Created by LENOVO on 30.09.2022.
//

#ifndef TOF_TOFDEVICE_H
#define TOF_TOFDEVICE_H

#include "IAcquirable.h"

#define PIXEL_FORMAT Coord3D_C16

class ToFDevice : IAcquirable<Arena::IImage>{
    Arena::IDevice* pDevice;
    Arena::IImage* pImage;
public:
    ToFDevice(Arena::IDevice* pDevice) : pDevice(pDevice) {}

    Arena::IImage* acquire() override {
        this->pImage = pDevice->GetImage(2000);
        return pImage;
    }

    int save() override {
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
    int clean() override {
        pDevice->RequeueBuffer(pImage);
        return 0;
    }
};

#endif //TOF_TOFDEVICE_H
