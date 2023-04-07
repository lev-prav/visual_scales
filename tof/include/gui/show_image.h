#pragma once
#include "imgui.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL header


// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);