////
//// Created by lev on 05.04.23.
////
//#include "BaseSaver.h"
//#include <sstream>
//#include <chrono>
//#include <iostream>
//
//void BaseSaver::log(const std::string &fname) {
//    using namespace std::chrono;
//
//    milliseconds ms = duration_cast< milliseconds>(system_clock::now().time_since_epoch());
//
//    time_t seconds = time(NULL);
//    tm *timeinfo = localtime(&seconds);
//    char timestamp[20];
//
//    strftime(timestamp, 20, "S %T", timeinfo);
//
//    std::cout << '[' << timestamp << "] " << fname << "\n";
//    fout << '[' << timestamp << "] " << fname << "\n";
//}
//
//int BaseSaver::run() {
//    using namespace std::chrono;
//    if (work){
//        return EXIT_FAILURE;
//    }
//
//    work = true;
//
//    acquisition_thread = std::thread([this](){
//        while(work){
//            milliseconds ms = duration_cast< milliseconds>(system_clock::now().time_since_epoch());
//
//            std::stringstream fname_stream;
//            fname_stream<<base_filename<<counter<<"_"<<ms.count()<<".tiff";
//            std::string fname = fname_stream.str();
//
//            log(fname);
//
//            auto read_data = bufferReader_->get_data();
//            if (!read_data.has_value()){
//                continue;
//            }
//            auto image = read_data.value();
//
//            saveImage(fname, image);
//
//            bufferReader_->move_forward();
//            counter++;
//        }
//    });
//
//    return 0;
//}
//
//int BaseSaver::stop() {
//    work = false;
//    if (acquisition_thread.joinable())
//        acquisition_thread.join();
//    return 0;
//}
