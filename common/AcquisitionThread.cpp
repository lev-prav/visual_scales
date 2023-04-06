#include <iostream>
#include "AcquisitionThread.h"

void AcquisitionThread::run() {

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

void AcquisitionThread::stop() {
    isRunning = false;
    if (thread.joinable())
        thread.join();
}
