//
// Created by lev on 29.09.22.
//

#ifndef TOF_ACQUISITIONTHREAD_H
#define TOF_ACQUISITIONTHREAD_H

template<class T>
class AcquisitionThread {
    T* dataSource;

public:
    AcquisitionThread(T* dataSource) : dataSource(dataSource){}

    int run(){

    }
};


#endif //TOF_ACQUISITIONTHREAD_H
