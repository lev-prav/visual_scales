//
// Created by LENOVO on 03.10.2022.
//

#ifndef TOF_TOFSAVER_H
#define TOF_TOFSAVER_H

#include "ISaver.h"
#include "ArenaApi.h"
#include "SaveApi.h"
#include <string>
#include <iostream>
#include <fstream>

#define PIXEL_FORMAT Coord3D_C16
namespace ToF{
class ToFSaver {
public:
    ToFSaver(const std::string &directory) :
            base_filename(directory + '/' + "image_"),
            counter(0),
            fout("log.txt") {}

    int save(Arena::IImage *pImage);

private:
    std::string base_filename;
    int counter;
    std::ofstream fout;

    int saveImage(const std::string &filename, Arena::IImage *pImage);
};


}
#endif //TOF_TOFSAVER_H
