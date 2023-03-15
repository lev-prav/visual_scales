#include "../include/ToFSaver.h"
#include <fstream>
#include <chrono>

int ToF::ToFSaver::run() {
    using namespace std::chrono;

    while(work){
        milliseconds ms = duration_cast< milliseconds>(system_clock::now().time_since_epoch());

        std::stringstream fname_stream;
        fname_stream<<base_filename<<counter<<"_"<<ms.count()<<".tiff";
        std::string fname = fname_stream.str();

        log(fname);

        auto read_data = bufferReader_->get_data();
        if (!read_data.has_value()){
            continue;
        }
        auto image = read_data.value();

        saveImage(fname, image);

        bufferReader_->move_forward();
        counter++;
    }

    return 0;
}

int ToF::ToFSaver::saveImage(const std::string &filename, const Image& image)  {

    Save::ImageParams params(
            image.width,
            image.height,
            image.bits_per_pixel);

    Save::ImageWriter writer(
            params,
            filename.c_str());

    // Save image
    //    Passing image data into the image writer using the cascading I/O
    //    operator (<<) triggers a save. Notice that the << operator accepts the
    //    image data as a constant unsigned 8-bit integer pointer (const
    //    uint8_t*) and the file name as a character string (const char*).

    writer << image.data.get();
}

void ToF::ToFSaver::log(const std::string& fname) {
    using namespace std::chrono;

    milliseconds ms = duration_cast< milliseconds>(system_clock::now().time_since_epoch());

    time_t seconds = time(NULL);
    tm *timeinfo = localtime(&seconds);
    char timestamp[20];

    strftime(timestamp, 20, "S %T", timeinfo);

    std::cout << '[' << timestamp << "] " << fname << "\n";
    fout << '[' << timestamp << "] " << fname << "\n";
}

int ToF::ToFSaver::read_buffer() {

    return 0;
}
