#include "appgl/gl/gl.hpp"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>

#include <spdlog/spdlog.h>
#include <stb_image.h>

#include "appgl/logging.hpp"

#if defined(_MSC_VER)
    //  Microsoft 
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
    #define __stdcall
#else
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

extern "C"
{
    EXPORT unsigned long NvOptimusEnablement = 0x00000001;
    EXPORT int AmdPowerXpressRequestHighPerformance = 1;

    static void __stdcall glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        auto logger = static_cast<spdlog::logger*>(const_cast<void*>(userParam));

        std::string source_name = "UNKNOWN";
        switch (source) {
        case GL_DEBUG_SOURCE_API:
            source_name = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            source_name = "WINDOW_SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            source_name = "SHADER_COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            source_name = "THIRD_PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            source_name = "APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            source_name = "OTHER";
            break;
        }

        std::string type_name = "UNKNOWN";
        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            type_name = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            type_name = "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            type_name = "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            type_name = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            type_name = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            type_name = "OTHER";
            break;
        }

        auto level = spdlog::level::trace;
        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            level = spdlog::level::trace;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            level = spdlog::level::debug;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            level = spdlog::level::warn;
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            level = spdlog::level::err;
            break;
        }

        logger->log(level, "[{}] {} Type: {} Id: {}", source_name, message, type_name, id);
    }
}

namespace appgl {

void print_opengl_info()
{
    printf("----------------------------------------------------------------\n");
    printf("OpenGL Info\n");
    printf("    Version: %s\n", glGetString(GL_VERSION));
    printf("     Vendor: %s\n", glGetString(GL_VENDOR));
    printf("   Renderer: %s\n", glGetString(GL_RENDERER));
    printf("    Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("----------------------------------------------------------------\n");
}

bool gl_init() {
    auto logger = logging::create("GL", spdlog::level::trace);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        spdlog::error("GLEW is not initialized!\n");
        return false;
    }

#if defined(DEBUG_OPENGL)
    print_opengl_info();

    if (glDebugMessageCallback)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugCallback, logger.get());

        GLuint unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
        
        logger->info("Debug callback registered\n");
    }
    else
    {
        spdlog::error("glDebugMessageCallback unavailable...\n");
    }
#endif
    
    return true;
}

GLenum gl_image_format(int channels) {
    switch (channels) {
        case 1: return GL_RED;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return 0;
    }
}

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path) {
    std::string vertex_source;
    std::string fragment_source;
    std::ifstream vertex_file;
    std::ifstream fragment_file;
    
    vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Open files
        vertex_file.open(vertex_path);
        fragment_file.open(fragment_path);
        std::stringstream vertex_stream, fragment_stream;
        // Read file's buffer contents into streams
        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();
        // Close file handlers
        vertex_file.close();
        fragment_file.close();
        // Convert stream into string
        vertex_source = vertex_stream.str();
        fragment_source = fragment_stream.str();
    } catch (std::ifstream::failure& e) {
        appgl::logger("GL")->error("SHADER::FILE_NOT_SUCCESSFULLY_READ");
    }

    const char* vertex_source_c = vertex_source.c_str();
    const char* fragment_source_c = fragment_source.c_str();

    // 2. Compile shaders
    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_source_c, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, vertex_path);

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_source_c, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, fragment_path);

    // Shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    check_link_errors(id);

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::check_compile_errors(GLuint shader, const std::string& name) {
    int success;
    char info_log[1024];
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        appgl::logger("GL")->error("SHADER_COMPILATION_ERROR({}): {}", name, info_log);
    }
}

void Shader::check_link_errors(GLuint shader) {
    int success;
    char info_log[1024];
    
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        appgl::logger("GL")->error("PROGRAM_LINKING_ERROR of type: {}", info_log);
    }
}

Texture::Texture() : id(-1), width(0), height(0), format(0) {
    glGenTextures(1, &id);
}
Texture::Texture(Texture&& other) : id(other.id), width(other.width), height(other.height), format(other.format) {
    other.id = -1;
}
Texture::~Texture() {
    if (id != -1) glDeleteTextures(1, &id);
}

// Load texture image data from a file
bool Texture::load(const std::string& image_path) {
    // Load image data
    int image_width, image_height, image_channels;
    unsigned char* data = stbi_load(image_path.c_str(), &image_width, &image_height, &image_channels, 0);
    if (!data) {
        appgl::logger("GL")->error("Failed to load texture at {}", image_path);
        return false;
    }

    auto image_format = gl_image_format(image_channels);
    if (!update(data, image_width, image_height, image_format)) {
        appgl::logger("GL")->error("Unsupported image format.");
        stbi_image_free(data);
        return false;
    }

    // Free the image data as it's no longer needed
    stbi_image_free(data);
    return true;
}

void Texture::use() {
    glBindTexture(GL_TEXTURE_2D, id);
}

// Update the texture with new image data
bool Texture::update(const unsigned char* data, int new_width, int new_height, GLenum new_format, GLint wrap, GLint filter, GLint type) {
    if (!data) {
        appgl::logger("GL")->error("Invalid data provided for texture update.");
        return false;
    }

    if (!new_width) new_width = width;
    if (!new_height) new_height = height;
    if (!new_format) new_format = format;

    if (new_format == 0) {
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, id);

    if (new_width == width && new_height == height && new_format == format) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, data);
    } else {
        width = new_width;
        height = new_height;
        format = new_format;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    return true;
}

} // namespace appgl
