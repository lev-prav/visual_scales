#include <iostream>
#include "../../include/gui/Viewer.h"
#include "../../include/gui/Texture.h"
#include <cstdio>

static int counter = 0;


ImGuiIO& Viewer::configure_context(GLFWwindow* window){
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();


    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return io;
}

void Viewer::create_stream_window(const GLuint &texture) {
    // MY IMAGE

    ImGui::Begin("Stream");
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[counter%2]);
    int im_width = 640, im_height = 480;
    update_image(im_width, im_height);

    ImGui::Text("size = %d x %d", im_width, im_height);
    ImGui::Text("Scanner state: %s", (activated) ? "ON" : "OFF");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Image((void*)(intptr_t)texture, ImVec2(im_width, im_height));
    if (ImGui::Button((!activated) ? "Start scanning" : "Stop scanning")){
        // Buttons return true when clicked (most widgets return true when edited/activated)
        activated = !activated;
        onActivate();
    }
    ImGui::SameLine();
    if (ImGui::Button("Somebody STOP ME") )
        stop_view = true;
    ImGui::End();

    //glDeleteTextures(1, &texture);
}


void Viewer::rendering(GLFWwindow *window, ImGuiIO &io) {
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Viewer::stop(GLFWwindow *window) {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

int Viewer::run() {
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    int win_height = 720, win_width = 1280;

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(win_width, win_height, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);

    ImGuiIO& io = configure_context(window);

    Texture texture = Texture::create_texture();

    // Main loop
    while (!glfwWindowShouldClose(window) and !stop_view)
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        create_stream_window(texture.get_texture_id());

        rendering(window, io);

        glfwSwapBuffers(window);
    }

    stop(window);

    return 0;
}


int Viewer::update_image(int& im_width, int& im_height) {
    auto read_data = bufferReader_->get_data();
    if (!read_data.has_value()){
        return 1;
    }

    auto image = read_data.value();
    auto raw_image = image.data.get();

    im_width = image.width, im_height = image.height;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16,
                 image.width, image.height, 0,
                 GL_LUMINANCE, GL_UNSIGNED_SHORT,
                 raw_image);

    bufferReader_->move_forward();

//    unsigned short* ptr_16 = reinterpret_cast<unsigned short *>(raw_image);
//    std::cout<<"\n";
//    for(int i = 0; i < 640; i++){
//        std::cout<<ptr_16[i]<<" ";
//    }
//    std::cout<<"\n";

    return 0;
}

void Viewer::onActivate() {
    activation_callback(activated);
}

void Viewer::set_activation_listener() {

}
