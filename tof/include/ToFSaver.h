//
// Created by LENOVO on 03.10.2022.
//

#ifndef TOF_TOFSAVER_H
#define TOF_TOFSAVER_H

#include "ArenaApi.h"
#include "SaveApi.h"
#include "buffer/BufferReader.h"
#include "../../scanner/BaseSaver.h"
#include <string>

namespace ToF{
class ToFSaver : public BaseSaver<Image>{
public:
    ToFSaver(const std::string &directory, std::shared_ptr<BufferReader<Image>> reader) :
            BaseSaver(directory, reader, "tof_", "tof_log.txt")
            {}

protected:

    int saveImage(const std::string &filename, const Image& image) override;
};


}
#endif //TOF_TOFSAVER_H
