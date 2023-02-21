#include "../include/ToFSensor.h"


int ToF::ToFSensor::connect() {
    pSystem = Arena::OpenSystem();
    pSystem->UpdateDevices(100);
    systemDevices = pSystem->GetDevices();
    if (systemDevices.empty()) {
        std::cout << "\nNo camera connected\nPress enter to complete\n";
        std::getchar();
        return EXIT_FAILURE;
    }
    std::cout << "ToF sensor successfully Connected!\n";
    return EXIT_SUCCESS;
}

int ToF::ToFSensor::startStream(const std::string& serialNumber) {
    if (pSystem == nullptr) return EXIT_FAILURE;

    auto deviceInfo = getToFDevice(serialNumber);

    if (deviceInfo == systemDevices.end()){
        throw std::invalid_argument("NO TOF SENSOR " + serialNumber);
    }
    int k = 0 ;
    for(auto i = systemDevices.begin(); i != systemDevices.end(); i++, k++){
        if (deviceInfo == i){
            break;
        }
    }

    //Arena::IDevice* pDevice = pSystem->CreateDevice(systemDevices[deviceNumber]);
    auto device = std::make_shared<ToFDevice>(pSystem->CreateDevice(*deviceInfo));
    std::cout << "Device Preparation..\n";
    device->prepareDevice();
    devices.push_back(device);

    auto acqThread = std::make_unique<AcquisitionThread>(device);
    acqThread->run();

    acqThreads.push_back(std::move(acqThread));
    std::cout << "Device number " << serialNumber << "starts streaming...\n";

    return 0;
}

int ToF::ToFSensor::stopStream(int deviceNumber) {
    if (deviceNumber >= acqThreads.size() or deviceNumber < 0)
        throw std::out_of_range("Device number out of range (" + std::to_string(deviceNumber) + ")");

    acqThreads[deviceNumber]->stop();
    std::cout << "Device number " << deviceNumber << "stop streaming...\n";

    return 0;
}

ToF::ToFSensor::~ToFSensor() {
    for(auto& device : devices){
        pSystem->DestroyDevice(device->getDevicePtr());
    }
    Arena::CloseSystem(pSystem);
}

std::vector<Arena::DeviceInfo>::iterator ToF::ToFSensor::getToFDevice(const std::string& serialNumber) {
    GenICam::gcstring serialToFind(serialNumber.c_str());

    auto it = std::find_if(begin(systemDevices), end(systemDevices), [&serialToFind](Arena::DeviceInfo deviceInfo)
    {
        return deviceInfo.SerialNumber() == serialToFind;
    });

    return it;
}
