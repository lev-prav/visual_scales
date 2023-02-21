#include "../include/AcquisitionThread.h"

void ToF::AcquisitionThread::run() {

    thread = std::thread([&](){
        dataSource->start();

        while(isRunning){
            dataSource->acquire();
            dataSource->save();
            dataSource->clean();
        }

        dataSource->stop();
    });

}

void ToF::AcquisitionThread::stop() {
    isRunning = false;
    thread.join();
}
