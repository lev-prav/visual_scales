//
// Created by LENOVO on 03.10.2022.
//

#ifndef TOF_TOFSAVER_H
#define TOF_TOFSAVER_H

#include "ISaver.h"
#include "ArenaApi.h"
#include "SaveApi.h"
#include <string>

#define PIXEL_FORMAT Coord3D_C16

class ToFSaver {
public:
    ToFSaver(const std::string& directory):
    base_filename(directory + '/' + "image_"),
    counter(0) {}

    int save(Arena::IImage* pImage)  {
        std::string fname = base_filename  + std::to_string(counter) + std::string(".tiff");

        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        char timestamp[20];
        strftime(timestamp, 20, "S %T", timeinfo);

        std::cout<<'['<<timestamp<<"] "<<fname<<"\n";
        saveImage(fname, pImage);
        counter++;
        return 0;
    }

private:
    std::string base_filename;
    int counter;

    int saveImage(const std::string& filename, Arena::IImage* pImage){
        auto pConverted = Arena::ImageFactory::Convert(
                pImage,
                PIXEL_FORMAT);

        Save::ImageParams params(
                pConverted->GetWidth(),
                pConverted->GetHeight(),
                pConverted->GetBitsPerPixel());

        Save::ImageWriter writer(
                params,
                filename.c_str());

        // Save image
        //    Passing image data into the image writer using the cascading I/O
        //    operator (<<) triggers a save. Notice that the << operator accepts the
        //    image data as a constant unsigned 8-bit integer pointer (const
        //    uint8_t*) and the file name as a character string (const char*).

        writer << pConverted->GetData();
        // destroy converted image
        Arena::ImageFactory::Destroy(pConverted);
    }
};

#endif //TOF_TOFSAVER_H
