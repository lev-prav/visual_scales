#ifndef TOF_VIEWER_H
#define TOF_VIEWER_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include "GLFW/glfw3.h" // Will drag system OpenGL headers
#include <utility>
#include <vector>

#include "stb_image.h"
#include "../buffer/BufferReader.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


class Viewer {
public:
    explicit Viewer(std::shared_ptr<BufferReader> reader) : bufferReader_(std::move(reader)) {}

    int run();
private:
    ImGuiIO& configure_context(GLFWwindow* window);

//    void create_stream_window( unsigned char** images );
    int update_image(int& im_width, int& im_height);
    void create_stream_window(const GLuint &textur);
    void rendering(GLFWwindow* window, ImGuiIO& io);
    void stop(GLFWwindow* window);

    static void glfw_error_callback(int error, const char* description){
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    std::shared_ptr<BufferReader> bufferReader_;
    bool activated = false;
    bool stop_view = false;
};


#endif //TOF_VIEWER_H
