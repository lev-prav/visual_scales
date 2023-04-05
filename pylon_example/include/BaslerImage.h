//
// Created by lev on 05.04.23.
//

#ifndef VISUAL_SCALES_BASLERIMAGE_H
#define VISUAL_SCALES_BASLERIMAGE_H


#include <pylon/GrabResultPtr.h>
#include "Image.h"

struct BaslerImage {
    int camera_index;
    int counter;
    Pylon::CGrabResultPtr ptrGrabResult;
};


#endif //VISUAL_SCALES_BASLERIMAGE_H
