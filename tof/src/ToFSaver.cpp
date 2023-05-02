#include "../include/ToFSaver.h"
#include <fstream>

#define PIXEL_FORMAT Coord3D_C16


int ToF::ToFSaver::saveImage(const std::string &filename, const Image& image)  {

    Save::ImageParams params(
            image.width,
            image.height,
            image.bits_per_pixel);
    std::stringstream fname_stream;
    fname_stream << base_filename << "_" << image.id <<"_"<<image.timestemp<< "_.tiff";
    Save::ImageWriter writer(
            params,
            fname_stream.str().c_str());

    int data_size = image.width*image.height*(image.bits_per_pixel/8);

    auto* save_image_data = new uint8_t [data_size];
    std::memcpy(save_image_data,image.data.get(), data_size);
    //auto* colors = new
    writer.SetTiff();
    writer.Save(save_image_data);
    //writer << save_image_data;
    delete[] save_image_data;

    return 0;
}

