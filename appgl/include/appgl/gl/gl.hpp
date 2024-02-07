#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>

#include <string>

namespace appgl {

static inline const char* gl_error_to_string(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR:          return "No error";
    case GL_INVALID_ENUM:      return "Invalid enum";
    case GL_INVALID_VALUE:     return "Invalid value";
    case GL_INVALID_OPERATION: return "Invalid operation";
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
    case GL_OUT_OF_MEMORY:     return "Out of memory";
    default:                   return "Unknown error";
    }
}

bool gl_init();

GLenum gl_image_format(int channels);

class Shader {
public:
    unsigned int id;

    Shader(const std::string& vertex_path, const std::string& fragment_path);

    void use();

    void check_compile_errors(GLuint shader, const std::string& type);
    void check_link_errors(GLuint shader);
};

struct Texture {
    unsigned int id;
    int width, height;
    GLenum format;

    Texture();
    ~Texture();
    
    bool load(const std::string& image_path);
    bool update(unsigned char* data, int new_width = 0, int new_height = 0, GLenum new_format = 0, GLint wrap = GL_REPEAT, GLint filter = GL_LINEAR, GLint type = GL_UNSIGNED_BYTE);
    
    void use();
};

} // namespace appgl
