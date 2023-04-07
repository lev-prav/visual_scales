cmake_minimum_required(VERSION 3.24)

project(visual_scales)

set(CMAKE_CXX_STANDARD 17)


message(STATUS ${Arena_FOUND})

set(ARENA_DIR /home/lev/data/ArenaSDK/ArenaSDK_Linux_x64)
set(THIRD_PARTY tof/third-party)
set(HEADER_DIR tof/include)
set(COMMON_DIR common)
set(SRC_DIR tof/src)

set(HEADERS ${HEADER_DIR}/ToFSensor.h
        ${COMMON_DIR}/AcquisitionThread.h
        ${COMMON_DIR}/IDevice.h
        ${COMMON_DIR}/Image.h
        ${COMMON_DIR}/buffer/Buffer.h
        ${COMMON_DIR}/buffer/BufferReader.h
        ${HEADER_DIR}/ToFDevice.h
        ${HEADER_DIR}/ToFSaver.h
        ${HEADER_DIR}/gui/show_image.h
        ${THIRD_PARTY}/stb/stb_image.h
        ${THIRD_PARTY}/imgui/backends/imgui_impl_glfw.h
        ${THIRD_PARTY}/imgui/backends/imgui_impl_opengl3.h
        ${HEADER_DIR}/gui/Viewer.h
        ${HEADER_DIR}/gui/Texture.h
        )

set(SOURCES ${HEADERS}
        ${COMMON_DIR}/AcquisitionThread.cpp
        ${COMMON_DIR}/buffer/Buffer.cpp
        ${COMMON_DIR}/buffer/BufferReader.cpp
        ${SRC_DIR}/ToFSensor.cpp
        ${SRC_DIR}/ToFSaver.cpp
        ${THIRD_PARTY}/imgui/backends/imgui_impl_glfw.cpp
        ${THIRD_PARTY}/imgui/backends/imgui_impl_opengl3.cpp
        ${SRC_DIR}/gui/show_image.cpp
        ${SRC_DIR}/gui/Viewer.cpp
        ${SRC_DIR}/ToFDevice.cpp
        )

add_library(tof_lib ${SOURCES} )

target_include_directories( tof_lib PUBLIC
        ${ARENA_DIR}/include/Arena
        ${ARENA_DIR}/include/Save
        ${ARENA_DIR}/include/GenTL
        ${ARENA_DIR}/GenICam/library/CPP/include
        ${THIRD_PARTY}/imgui
        ${THIRD_PARTY}/stb
        ${THIRD_PARTY}/imgui/backends
        tof/cmake-build-debug/_deps/glfw-src/include
)

set(LIB_DIRS
        ${ARENA_DIR}/GenICam/library/lib/Linux64_x64
        ${ARENA_DIR}/lib64
        ${ARENA_DIR}/ffmpeg
        ${THIRD_PARTY}/imgui/cmake-build-debug
        )

set(GENICAMLIBS GCBase_gcc54_v3_3_LUCID
        GenApi_gcc54_v3_3_LUCID
        Log_gcc54_v3_3_LUCID
        log4cpp_gcc54_v3_3_LUCID
        MathParser_gcc54_v3_3_LUCID
        NodeMapData_gcc54_v3_3_LUCID
        XmlParser_gcc54_v3_3_LUCID
        )


set(FFMPEGLIBS  avcodec
        avformat
        avutil
        swresample
        )

set(LIBS    arena
        save
        gentl
        ${GENICAMLIBS}
        ${FFMPEGLIBS}
        pthread
        lucidlog
        imgui
        GL)

add_subdirectory(tof/third-party/imgui)

message(${CMAKE_BINARY_DIR})

target_link_directories(tof_lib PUBLIC ${LIB_DIRS})
target_link_libraries(tof_lib PRIVATE ${LIBS})


# PYLON
set(BASLER_SOURCES  pylon_example/Grabber.h
                    pylon_example/main.cpp
        pylon_example/src/TwoCameras.cpp
        pylon_example/include/TwoCameras.h
        pylon_example/src/BaslerSaver.cpp
        pylon_example/include/BaslerSaver.h
        pylon_example/src/BaslerCamera.cpp
        pylon_example/include/BaslerCamera.h
        pylon_example/include/BaslerImage.h )
find_package(pylon 7.2.1 REQUIRED)

add_library(baslers_lib ${BASLER_SOURCES})
include_directories(scanner
                    ${COMMON_DIR}
                    /opt/pylon/include
                    tof/include)
target_link_libraries( baslers_lib PRIVATE pylon::pylon pthread)
# -----


set(SCANNER_SOURCES scanner/Scanner.h
        scanner/Scanner.cpp
        scanner/BaseSaver.cpp
        scanner/BaseSaver.h)

# RUN
add_executable(visual_scales main.cpp ${SCANNER_SOURCES} )

target_include_directories(visual_scales PUBLIC ${HEADER_DIR} ${THIRD_PARTY} )
target_link_libraries(visual_scales tof_lib baslers_lib)
