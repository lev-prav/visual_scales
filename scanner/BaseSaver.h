//
// Created by lev on 05.04.23.
//

#ifndef VISUAL_SCALES_BASESAVER_H
#define VISUAL_SCALES_BASESAVER_H

#include <string>
#include <memory>
#include <fstream>
#include <thread>
#include <iostream>
#include <sstream>
#include "buffer/BufferReader.h"
#include "Image.h"

template <class T>
class BaseSaver {
public:
    BaseSaver(const std::string &directory, std::shared_ptr<BufferReader<T>> reader, const std::string& base_file_name ,const std::string& log_file_name) :
            base_filename(directory + base_file_name),
            counter(0),
            fout(directory + log_file_name),
            bufferReader_(reader)
    {}

    int run(){
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
    int stop(){
        work = false;
        if (acquisition_thread.joinable())
            acquisition_thread.join();
        return 0;
    }
    virtual ~BaseSaver(){}

protected:
    virtual int saveImage(const std::string &filename, const T& image) = 0;

    void log(const std::string& fname){
        using namespace std::chrono;

        milliseconds ms = duration_cast< milliseconds>(system_clock::now().time_since_epoch());

        time_t seconds = time(NULL);
        tm *timeinfo = localtime(&seconds);
        char timestamp[20];

        strftime(timestamp, 20, "S %T", timeinfo);

        //std::cout << '[' << timestamp << "] " << fname << "\n";
        fout << '[' << timestamp << "] " << fname << "\n";
    };

    bool work = false;
    int counter;
    std::ofstream fout;
    std::string base_filename;
    std::shared_ptr<BufferReader<T>> bufferReader_;
    std::thread acquisition_thread;
};


#endif //VISUAL_SCALES_BASESAVER_H
