#pragma once
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

class Texture{
public:
    static Texture create_texture(){
        GLuint image_texture;
        return {image_texture};
    }

    Texture(GLuint texture) : texture_id(texture){
        configure_texture();
    }

    const GLuint& get_texture_id(){
        return texture_id;
    }

    ~Texture(){
        glDeleteTextures(1, &texture_id);
    }

private:
    Texture() = default;

    void configure_texture(){
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    }

    GLuint texture_id;
};

