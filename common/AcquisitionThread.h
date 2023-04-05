//
// Created by lev on 29.09.22.
//

#ifndef TOF_ACQUISITIONTHREAD_H
#define TOF_ACQUISITIONTHREAD_H

#include "IDevice.h"
#include <thread>
#include <atomic>
#include <utility>

class AcquisitionThread {
public:
    AcquisitionThread(std::shared_ptr<IDevice> dataSource) :
    dataSource(std::move(dataSource)) {}

    void run();
    void stop();

private:
    std::atomic<bool> isRunning{true};
    std::shared_ptr<IDevice> dataSource;
    std::thread thread;
};


#endif //TOF_ACQUISITIONTHREAD_H
