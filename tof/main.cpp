//
// Created by LENOVO on 03.10.2022.
//
#include "include/ToFSensor.h"
#include <chrono>
#include <imgui.h>
#include "../third-party/imgui/backends/imgui_impl_glfw.h"
#include "../third-party/imgui/backends/imgui_impl_opengl3.h"

int main(int argc, char** argv){
    std::string tofSerialNumber = "214100232";
    ToF::ToFSensor sensor;

    char buf[120];
    float f;
    bool opened = true;

    ImGui::Begin("Ваше окно", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
    static bool test = false;

    ImGui::Text("Hello, world %d", 123);
    if (ImGui::Button("Save"))
        std::cout<<"SAVE\n";
    ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    ImGui::End();
    ImGui::EndFrame();

    try {
        sensor.connect();
        //sensor.startStream(tofSerialNumber);
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