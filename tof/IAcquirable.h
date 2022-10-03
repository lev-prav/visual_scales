//
// Created by LENOVO on 30.09.2022.
//

#ifndef TOF_IACQUIRABLE_H
#define TOF_IACQUIRABLE_H

#include <string>

template<class T>
class IAcquirable{
public:
    virtual T acquire() = 0;
    virtual int save(T* data, const std::string&) = 0;
    virtual int clean() = 0;
};
#endif //TOF_IACQUIRABLE_H