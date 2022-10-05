//
// Created by lev on 29.09.22.
//

#ifndef TOF_ACQUISITIONTHREAD_H
#define TOF_ACQUISITIONTHREAD_H

#include "IDevice.h"
#include <thread>
#include <atomic>

class AcquisitionThread {
    IDevice* dataSource;
    std::atomic<bool> isRunning {true};
    std::thread* thread;
public:
    AcquisitionThread(IDevice* dataSource) : dataSource(dataSource){}

    void run(){

        thread = new std::thread([&](){
            dataSource->start();

            while(isRunning){
                dataSource->acquire();
                dataSource->save();
                dataSource->clean();
            }

            dataSource->stop();
        });

    }

    void stop(){
        isRunning = false;
        thread->join();
    }

    ~AcquisitionThread(){
        delete thread;
        delete dataSource;
    }
};


#endif //TOF_ACQUISITIONTHREAD_H
