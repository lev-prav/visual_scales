//
// Created by LENOVO on 03.10.2022.
//
#include "ToFSensor.h"

int main(){
    ToFSensor sensor;
    sensor.connect();
    sensor.startStream();

    std::getchar();

    sensor.stopStream();

    return 0;
}