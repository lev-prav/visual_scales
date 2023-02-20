//
// Created by LENOVO on 30.09.2022.
//

#ifndef TOF_IDEVICE_H
#define TOF_IDEVICE_H

#include <string>

class IDevice{
public:
    virtual void start() = 0;
    virtual void acquire() = 0;
    virtual void save() = 0;
    virtual void clean() = 0;
    virtual void stop() = 0;

    virtual ~IDevice() = default;;
};
#endif //TOF_IDEVICE_H