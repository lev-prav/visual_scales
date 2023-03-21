cmake_minimum_required(VERSION 3.16)

project(tof)


set(CMAKE_CXX_STANDARD 17)
set(ARENA_DIR /home/lev/data/ArenaSDK/ArenaSDK_Linux_x64)
set(THIRD_PARTY third-party)

include_directories(
        ${ARENA_DIR}/include/Arena
        ${ARENA_DIR}/include/Save
        ${ARENA_DIR}/include/GenTL
        ${ARENA_DIR}/GenICam/library/CPP/include
        ${THIRD_PARTY}/imgui
        ${THIRD_PARTY}/stb
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
            ${HEADER_DIR}/ToFSaver.h
            ${HEADER_DIR}/gui/show_image.h
            ${THIRD_PARTY}/stb/stb_image.h
            ${THIRD_PARTY}/imgui/backends/imgui_impl_glfw.h
            ${THIRD_PARTY}/imgui/backends/imgui_impl_opengl3.h
        )

set(SOURCES ${HEADERS}
            ${SRC_DIR}/ToFSensor.cpp
            ${SRC_DIR}/AcquisitionThread.cpp
            ${SRC_DIR}/ToFSaver.cpp
            ${THIRD_PARTY}/imgui/backends/imgui_impl_glfw.cpp
            ${THIRD_PARTY}/imgui/backends/imgui_impl_opengl3.cpp
            ${SRC_DIR}/gui/show_image.cpp
        src/gui/Viewer.cpp
        include/gui/Viewer.h
        include/Image.h
        include/gui/Texture.h
        src/buffer/Buffer.cpp
        include/buffer/Buffer.h
        src/ToFDevice.cpp
        include/buffer/BufferReader.h
        src/buffer/BufferReader.cpp)

#add_executable(tof  ${SOURCES}
#                    main.cpp)
#target_link_libraries(tof  ${LIBS} )


add_library(tof ${SOURCES})
target_link_libraries(tof ${LIBS})

