//
// Created by LENOVO on 03.10.2022.
//
#include "include/ToFSensor.h"
#include <chrono>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "third-party/imgui/backends/imgui_impl_glfw.h"
#include "third-party/imgui/backends/imgui_impl_opengl3.h"

int main(int argc, char** argv){
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

    int time = 5000;
    if (argc > 1){
        time = atoi(argv[1]);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(time));

    sensor.stopStream();

    return 0;
}