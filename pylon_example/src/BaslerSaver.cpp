//
// Created by lev on 05.04.23.
//

#include "../include/BaslerCamera.h"
#include "../include/BaslerSaver.h"
#include "../include/BaslerImage.h"

using namespace Pylon;
using namespace std;

int BaslerSaver::saveImage(const std::string &filename, const BaslerImage &image_) {

    auto& res_image = image_.image;
    std::stringstream fname_stream;
    fname_stream << base_filename << image_.camera_index << "_" << image_.id << "_.tiff";
    std::string path = fname_stream.str();
    std::cout << path<<"\n";
    try{
        res_image.Save(Pylon::ImageFileFormat_Tiff, path.c_str());
    }
    catch (const GenericException& e)
    {
        // Error handling
        cerr << "An exception occurred." << endl
             << e.GetDescription() << endl;

        return EXIT_FAILURE;
    }

    return 0;
}
