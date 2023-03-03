#include "../include/ToFSaver.h"
#include <fstream>
#include <chrono>

int ToF::ToFSaver::save(Arena::IImage *pImage) {
    using namespace std::chrono;

    milliseconds ms = duration_cast< milliseconds>(system_clock::now().time_since_epoch());

    std::stringstream fname_stream;
    fname_stream<<base_filename<<counter<<"_"<<ms.count()<<".tiff";
    //std::string fname = base_filename + std::to_string(counter) + "_" + seconds +std::string(".tiff");

    std::string fname = fname_stream.str();

    time_t seconds = time(NULL);
    tm *timeinfo = localtime(&seconds);
    char timestamp[20];

    strftime(timestamp, 20, "S %T", timeinfo);

    std::cout << '[' << timestamp << "] " << fname << "\n";
    fout << '[' << timestamp << "] " << fname << "\n";                            // write to file temporary

    saveImage(fname, pImage);
    counter++;
    return 0;
}

int ToF::ToFSaver::saveImage(const std::string &filename, Arena::IImage *pImage)  {
    auto pConverted = Arena::ImageFactory::Convert(
            pImage,
            PIXEL_FORMAT);
    auto data = pImage->GetData();
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