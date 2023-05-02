#include <iostream>
#include <thread>
#include "gui/Viewer.h"
#include "scanner/Scanner.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int run_lidar(const std::string& save_dir){
    //lev@dimbass-MS-7851:~/data/4Cameras/visual_scales/lidar/rplidar_sdk/output/Linux/Release$ ./ultra_simple --channel --tcp 192.168.0.7 20108 /home/lev/data/4Cameras/visual_scales/data/ 10000

    std::string file = "/home/lev/data/4Cameras/rplidar_sdk/output/Linux/Release/ultra_simple",
                channel = "--channel",
                protocol = "--tcp",
                ip = "192.168.0.7",
                port = "20108";
    std::string outfile = save_dir + "lidar_output.txt";

    auto pid=fork();

    std::stringstream lidar_exe;
    lidar_exe<<file<<" "<<channel<<" "<<protocol<<" "<<ip<<" "<<port;
    if (pid==-1){
        std::cout<<"Error ,process not fork"<<std::endl;
        return EXIT_FAILURE;
    }
    else if (pid==0)
    {
        std::cout <<"Lidar ON: "<<getpid()<<"\n";
        execlp(file.c_str(),  lidar_exe.str().c_str(), outfile.c_str(), NULL);
    }

    return pid;
}

int main(int argv, char** argc) {
    std::string tofSerialNumber = "214100232";
    std::string save_dir = "/home/lev/data/4Cameras/visual_scales/test/";


    Scanner scanner(tofSerialNumber, save_dir, 60);

    scanner.run();

    auto gui_thread = std::thread([&scanner](){
        auto reader = scanner.get_buffer_reader();

        Viewer viewer(reader);
        viewer.set_activation_listener(scanner.get_listener()); // create correct listener object
        viewer.set_fps_controller(scanner.get_fps_controller());

        viewer.run();
    });

    int lidar_code = -1;
    bool lidar_on = true;
    /*
    auto lidar_thread = std::thread([&save_dir, &lidar_code, &lidar_on](){
        //lev@dimbass-MS-7851:~/data/4Cameras/visual_scales/lidar/rplidar_sdk/output/Linux/Release$ ./ultra_simple --channel --tcp 192.168.0.7 20108 /home/lev/data/4Cameras/visual_scales/data/ 10000

        std::string file = "/home/lev/data/4Cameras/rplidar_sdk/output/Linux/Release/ultra_simple",
                channel = "--channel",
                protocol = "--tcp",
                ip = "192.168.0.7",
                port = "20108";
        std::string outfile = save_dir + "lidar_output.txt";

        auto pid=fork();
        lidar_code = pid;

        std::stringstream lidar_exe;
        lidar_exe<<file<<" "<<channel<<" "<<protocol<<" "<<ip<<" "<<port;
        std::cout<<lidar_exe.str()<<"\n";
        if (pid==-1){
            std::cout<<"Error ,process not fork"<<std::endl;
        }
        else if (pid==0)
        {
            std::cout <<"Lidar ON: "<<getpid()<<"\n";
            execlp(file.c_str(),  file.c_str(),
                   channel.c_str(),
                   protocol.c_str(),
                   ip.c_str(),
                   port.c_str(),
                   outfile.c_str(), NULL);

            while(lidar_on){}
        }

    });
    */

    if (lidar_code != 0){
        gui_thread.join();
        scanner.stop();
        lidar_on = false;
    }

    //lidar_thread.join();

    return 0;
}
