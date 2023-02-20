//
// Created by LENOVO on 03.10.2022.
//
#include "include/ToFSensor.h"
#include <chrono>

int main(){
    std::string tofSerialNumber = "214100232";
    ToF::ToFSensor sensor;

    try {
        sensor.connect();
        sensor.startStream(tofSerialNumber);
    }
    catch (GenICam::GenericException& ge)
    {
        std::cout << "\nGenICam exception thrown: " << ge.what() << "\n";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    sensor.stopStream();

    return 0;
}