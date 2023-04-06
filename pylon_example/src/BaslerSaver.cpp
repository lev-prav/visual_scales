//
// Created by lev on 05.04.23.
//

#include "../include/BaslerCamera.h"
#include "../include/BaslerSaver.h"
#include "../include/BaslerImage.h"

using namespace Pylon;
using namespace std;

int BaslerSaver::saveImage(const std::string &filename, const BaslerImage &image_) {

    CPylonImage image;
    image.AttachGrabResultBuffer( image_.ptrGrabResult);
    std::stringstream fname_stream;
    fname_stream << base_filename << image_.camera_index << "_" << counter << "_.tiff";
    std::string path = fname_stream.str();
    std::cout << path<<"\n";
    try{
        image.Save(Pylon::ImageFileFormat_Tiff, path.c_str());
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
