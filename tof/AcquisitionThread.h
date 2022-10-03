//
// Created by lev on 29.09.22.
//

#include "IAcquirable.h"

#ifndef TOF_ACQUISITIONTHREAD_H
#define TOF_ACQUISITIONTHREAD_H

template<class T>
class AcquisitionThread {
    IAcquirable<T>* dataSource;
public:
    AcquisitionThread(IAcquirable<T>* dataSource) : dataSource(dataSource){}

    int run(bool* isRunning){
        while(*isRunning){
            auto data = dataSource->acquire();
            dataSource->save(data);
            dataSource->clean();
        }
    }
};


#endif //TOF_ACQUISITIONTHREAD_H
