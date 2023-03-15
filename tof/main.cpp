#include "include/gui/Viewer.h"
#include "include/ToFSensor.h"
#include "include/ToFSaver.h"

int main(int argc, char** argv){
    unsigned int buffer_size = 60;
    std::string tofSerialNumber = "214100232";
    std::string save_dir = "/home/lev/data/4Cameras/visual_scales/test/";

    int time = 5000;
    if (argc > 1){
        time = atoi(argv[1]);
    }

    std::shared_ptr<Buffer> buffer_ptr = std::make_shared<Buffer>(buffer_size);

    auto tof_thread = std::thread([&](){
        ToF::ToFSensor sensor(buffer_ptr);
        try {
            sensor.connect();
            sensor.startStream(tofSerialNumber);
        }
        catch (GenICam::GenericException& ge)
        {
            std::cout << "\nGenICam exception thrown: " << ge.what() << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(time));

        sensor.stopStream();
    });

    auto save_thread = std::thread([&buffer_ptr, &save_dir](){
        auto reader = buffer_ptr->get_reader();

        ToF::ToFSaver saver(save_dir, reader);
        saver.run();
    });

    auto gui_thread = std::thread([&buffer_ptr](){
        auto reader = buffer_ptr->get_reader();

        Viewer viewer(reader);
        viewer.run();
    });

    tof_thread.join();
    gui_thread.join();


    return 0;
}