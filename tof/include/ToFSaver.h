//
// Created by LENOVO on 03.10.2022.
//

#ifndef TOF_TOFSAVER_H
#define TOF_TOFSAVER_H

#include "ISaver.h"
#include "ArenaApi.h"
#include "SaveApi.h"
#include "buffer/BufferReader.h"
#include "Image.h"
#include <string>
#include <iostream>
#include <fstream>

#define PIXEL_FORMAT Coord3D_C16
namespace ToF{
class ToFSaver {
public:
    ToFSaver(const std::string &directory, std::shared_ptr<BufferReader> reader) :
            base_filename(directory + "image_"),
            counter(0),
            fout(directory + "tof_log.txt"),
            bufferReader_(reader)
            {}

    int run();

private:

    int saveImage(const std::string &filename, const Image& image);
    void log(const std::string& fname);
    int read_buffer();

    bool work = true;
    int counter;
    std::ofstream fout;
    std::string base_filename;
    std::shared_ptr<BufferReader> bufferReader_;

};


}
#endif //TOF_TOFSAVER_H
