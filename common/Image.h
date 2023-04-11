#ifndef TOF_IMAGE_H
#define TOF_IMAGE_H
#include <memory>

struct Image{
    unsigned long long id ;
    int width;
    int height;
    int bits_per_pixel;
    std::shared_ptr<unsigned char[]> data;
    long timestemp;
};

#endif //TOF_IMAGE_H
