cmake_minimum_required(VERSION 3.20)

project(imgui)

option(BUILD_OPENGL3_EXAMPLE "Build opengl3 backend imgui demo" ON)

add_library(imgui
        "imgui.h"
        "imgui_internal.h"
        "imgui.cpp"
        "imgui_demo.cpp"
        "imgui_draw.cpp"
        "imgui_widgets.cpp"
        "imgui_tables.cpp"
        "backends/imgui_impl_opengl3.h"
        "backends/imgui_impl_opengl3.cpp"
        "backends/imgui_impl_glfw.h"
        "backends/imgui_impl_glfw.cpp"
        )
add_library(imgui::imgui ALIAS imgui)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(imgui PUBLIC "backends/")

target_compile_definitions(imgui PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GL3W=1
        IMGUI_DISABLE_OBSOLETE_FUNCTIONS # to check for obsolete functions
        # IMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/path/to/imconfig.h" # to use your own imconfig.h
        )

# the following should go in different files, but we're keeping things here to avoid future conflicts.
if(NOT TARGET glfw)
    include(FetchContent)
    FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG tags/3.3
            GIT_SHALLOW TRUE
    )
    option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
    option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
    option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
    option(GLFW_INSTALL "Generate installation target" OFF)
    option(GLFW_VULKAN_STATIC "Use the Vulkan loader statically linked into application" OFF)
    FetchContent_MakeAvailable(glfw)
endif()

target_link_libraries(imgui PUBLIC glfw)

if (BUILD_OPENGL3_EXAMPLE)
    message(STATUS "Building imgui opengl3 example")
    add_executable(imgui_example "examples/example_glfw_opengl3/main.cpp")
    target_link_libraries(imgui_example PRIVATE imgui)
    find_package(OpenGL REQUIRED)
    if(TARGET OpenGL::OpenGL)
        message(STATUS Opengl)
        target_link_libraries(imgui_example PRIVATE OpenGL::OpenGL ${CMAKE_DL_LIBS})
    else()
        message(STATUS gl)
        target_link_libraries(imgui_example PRIVATE OpenGL::GL ${CMAKE_DL_LIBS})
    endif()
endif()
