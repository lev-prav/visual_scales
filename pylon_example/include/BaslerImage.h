//
// Created by lev on 05.04.23.
//

#ifndef VISUAL_SCALES_BASLERIMAGE_H
#define VISUAL_SCALES_BASLERIMAGE_H


#include <pylon/GrabResultPtr.h>
#include "Image.h"

struct BaslerImage {
    unsigned long long id ;
    int camera_index;
    Pylon::CPylonImage image;
    long timestemp;
};


#endif //VISUAL_SCALES_BASLERIMAGE_H
