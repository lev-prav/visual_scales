cmake_minimum_required(VERSION 3.16)

project(tof)

set(CMAKE_CXX_STANDARD 17)
set(ARENA_DIR /home/lev/data/ArenaSDK/ArenaSDK_Linux_x64)
set(IMGUI_DIR third-party/imgui)

include_directories(
        ${ARENA_DIR}/include/Arena
        ${ARENA_DIR}/include/Save
        ${ARENA_DIR}/include/GenTL
        ${ARENA_DIR}/GenICam/library/CPP/include
        ${IMGUI_DIR}
)

set(LIB_DIRS
        ${ARENA_DIR}/GenICam/library/lib/Linux64_x64
        ${ARENA_DIR}/lib64
        ${ARENA_DIR}/ffmpeg
        ${IMGUI_DIR}/cmake-build-debug
        )

set(GENICAMLIBS GCBase_gcc54_v3_3_LUCID
                GenApi_gcc54_v3_3_LUCID
                Log_gcc54_v3_3_LUCID
                log4cpp_gcc54_v3_3_LUCID
                MathParser_gcc54_v3_3_LUCID
                NodeMapData_gcc54_v3_3_LUCID
                XmlParser_gcc54_v3_3_LUCID)


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

add_subdirectory(third-party/imgui)

link_directories(${LIB_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(HEADER_DIR include)
set(SRC_DIR src)

set(HEADERS ${HEADER_DIR}/ToFSensor.h
            ${HEADER_DIR}/AcquisitionThread.h
            ${HEADER_DIR}/IDevice.h
            ${HEADER_DIR}/ToFDevice.h
            ${HEADER_DIR}/ToFSaver.h )
set(SOURCES ${HEADERS}
            ${SRC_DIR}/ToFSensor.cpp
            ${SRC_DIR}/AcquisitionThread.cpp
            ${SRC_DIR}/ToFSaver.cpp
            ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
            ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
            ${IMGUI_DIR}/backends/imgui_impl_glfw.h
            ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
        )

add_executable(tof  ${SOURCES}
                    main.cpp)
target_link_libraries(tof  ${LIBS} )

#add_library(${LIBS})

