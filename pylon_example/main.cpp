#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>
#include <vector>
#include <thread>

using namespace Pylon;
using namespace std;

static const size_t c_maxCamerasToUse = 2;
static const uint32_t c_countOfImagesToGrab = 10;

using cam = Pylon::CBaslerUniversalInstantCamera;
using cam_ptr = std::shared_ptr<cam>;

int main( int argc, char* argv[] )
{
    bool work = true;
    std::string save_dir = "../images/";
    if (argc > 1){
        save_dir = argv[1];
    }
    auto stream = [&work, &save_dir](cam_ptr cam, int cam_index) {
        cam->StartGrabbing();

        using namespace std::chrono;

        CGrabResultPtr ptrGrabResult;
        int counter = 0;

        for (uint32_t i = 0; cam->IsGrabbing() && work; ++i, counter++) {
            cam->RetrieveResult(20000, ptrGrabResult);
            milliseconds ms = duration_cast< milliseconds>(system_clock::now().time_since_epoch());

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded()) {

                intptr_t cameraContextValue = ptrGrabResult->GetCameraContext();


                // Print the index and the model name of the camera.
                cout << "Camera " << cam_index << ": " << cam->GetDeviceInfo().GetModelName() <<"\n"
                        << "GrabSucceeded: " << ptrGrabResult->GrabSucceeded()<<"\n"
                        << "SizeX: " << ptrGrabResult->GetWidth() << "\n"
                        << "SizeY: " << ptrGrabResult->GetHeight() << "\n";

                CPylonImage image;
                image.AttachGrabResultBuffer( ptrGrabResult );
                std::stringstream fname_stream;
                fname_stream <<save_dir<< cam_index<<"_"<< counter<<"_"<<ms.count()<< ".tiff";
                std::string path = fname_stream.str();
                std::cout<<path;
                image.Save(Pylon::ImageFileFormat_Tiff,path.c_str());

               /* try {
                    CPylonImage image;
                    image.AttachGrabResultBuffer( ptrGrabResult );
                    std::stringstream fname_stream;
                    fname_stream <<save_dir<< cam_index<<"_"<< counter<<"_"<<ms.count()<< ".tiff";
                    std::string path = fname_stream.str();
                    std::cout<<path;
                    image.Save(Pylon::ImageFileFormat_Tiff,path.c_str());

                } catch(const GenericException& ex){
                    std::cout<<ex.what()<<"\n";
                }*/

            } else {
                cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " "
                     << ptrGrabResult->GetErrorDescription() << endl;
            }
        }

        cam->StopGrabbing();
    };

    // The exit code of the sample application.
    int exitCode = 0;

    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();

    // Get the transport layer factory.
    CTlFactory &tlFactory = CTlFactory::GetInstance();

    // Get all attached devices and exit application if no device is found.
    DeviceInfoList_t devices;
    if (tlFactory.EnumerateDevices(devices) == 0) {
        throw RUNTIME_EXCEPTION("No camera present.");
    }

    // Create an array of instant cameras for the found devices and avoid exceeding a maximum number of devices.
    std::vector<cam_ptr> cameras(min(devices.size(), c_maxCamerasToUse));

    std::cout << devices.size() << "\n";

    // Create and attach all Pylon Devices.
    for (size_t i = 0; i < cameras.size(); ++i) {
        auto dev = tlFactory.CreateDevice(devices[i]);

        cameras[i] = std::make_shared<cam>(dev);
        //cameras[i]->Attach(dev);

        auto open = cameras[i]->IsOpen();
        try{
            cameras[i]->Open();
        } catch(const GenericException& ex){
            std::cout<<ex.what()<<"\n";
        }

        cameras[i]->LineSelector.SetValue(Basler_UniversalCameraParams::LineSelector_Line1);
        cameras[i]->TriggerMode.SetValue(Basler_UniversalCameraParams::TriggerMode_On);
        cameras[i]->LineMode.SetValue(Basler_UniversalCameraParams::LineMode_Input);

        // Enable triggered image acquisition for the Frame Start trigger
        // Select the Frame Start trigger

        // Print the model name of the camera.
        cout << "Using device " << cameras[i]->GetDeviceInfo().GetModelName() << endl;
    }

    std::vector<std::thread> threads;

    for(int i = 0; i < cameras.size(); i++){
        threads.emplace_back(stream, cameras[i], i);
    }


    // Comment the following two lines to disable waiting on exit.
    std::cout<< endl << "Press enter to stop." << endl;
    while (cin.get() != '\n');

    work = false;

    for(int i = 0; i < cameras.size(); i++){
        threads[i].join();
        cameras[i]->Close();
    }

    // Releases all pylon resources.
    PylonTerminate();

    return exitCode;
}