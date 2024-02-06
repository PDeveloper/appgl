#include "appgl/gl.hpp"

#include <cstdio>
#include "spdlog/spdlog.h"

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
        auto logger = logging::create("OpenGL", spdlog::level::trace);

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

} // namespace appgl
