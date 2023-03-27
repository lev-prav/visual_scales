#include "../include/ToFSaver.h"
#include <fstream>
#include <chrono>

int ToF::ToFSaver::run() {
    using namespace std::chrono;
    if (work){
        return EXIT_FAILURE;
    }

    work = true;

    acquisition_thread = std::thread([this](){
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
    });

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

    int data_size = image.width*image.width*(image.bits_per_pixel/8);

    auto* save_image_data = new unsigned char[data_size];
    std::memcpy(save_image_data,image.data.get(), data_size);
    writer << save_image_data;
    delete[] save_image_data;
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

int ToF::ToFSaver::stop() {
    work = false;
    if (acquisition_thread.joinable())
        acquisition_thread.join();
    return 0;
}
