//
// Created by lev on 05.04.23.
//

#ifndef VISUAL_SCALES_BASLERSAVER_H
#define VISUAL_SCALES_BASLERSAVER_H


#include "buffer/BufferReader.h"
#include "Image.h"
#include "BaseSaver.h"

class BaslerSaver : public BaseSaver<BaslerImage> {
public:
    BaslerSaver(const std::string& directory, std::shared_ptr<BufferReader<BaslerImage>> reader)
            : BaseSaver(directory, reader, "cam_", "camera_save_log.txt"){}

protected:

    int saveImage(const std::string &filename, const BaslerImage& image) override;
};


#endif //VISUAL_SCALES_BASLERSAVER_H
