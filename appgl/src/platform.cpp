#include "appgl/platform.hpp"

extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

    static void __stdcall glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

        printf("-------------------- OpenGL Debug Callback -----------\n");
        printf("Message: %s\n", message);
        printf("Type: ");
        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            printf("ERROR");
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            printf("DEPRECATED_BEHAVIOR");
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            printf("UNDEFINED_BEHAVIOR");
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            printf("PORTABILITY");
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            printf("PERFORMANCE");
            break;
        case GL_DEBUG_TYPE_OTHER:
            printf("OTHER");
            break;
        default:
            printf("UNKNOWN");
            break;
        }
        printf("\n");

        printf("Id: %d\n", id);
        printf("Severity: ");
        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            printf("NOTIFICATION");
            break;
        case GL_DEBUG_SEVERITY_LOW:
            printf("LOW");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            printf("MEDIUM");
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            printf("HIGH");
            break;
        }
        printf("\n\n");
    }
}

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

Platform::Platform() {
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW");
        std::exit(EXIT_FAILURE);
    }
}
Platform::~Platform() {
    glfwTerminate();
}
void Platform::poll_events() {
    glfwPollEvents();
}
std::span<GLFWmonitor*> Platform::monitors() {
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    return std::span<GLFWmonitor*>(monitors, count);
}

void window_hints_gl() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void window_hints_fullscreen(const GLFWvidmode* mode) {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
}

Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
    window = glfwCreateWindow(width, height, title, monitor, share);
    if (!window) {
        spdlog::error("Failed to create GLFW window");
    }
}
Window::~Window() {
	glfwDestroyWindow(window);
}
void Window::make_current() {
    glfwMakeContextCurrent(window);
}
bool Window::should_close() {
    return glfwWindowShouldClose(window);
}
void Window::swap() {
    glfwSwapBuffers(window);
}
Window::operator GLFWwindow* () const {
    return window;
}
Window Window::create(int width, int height, const char* title) {
    window_hints_gl();
    return Window(width, height, title);
}
Window Window::create_fullscreen(const char* title, GLFWmonitor* monitor) {
    if (!monitor) monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    window_hints_gl();
    window_hints_fullscreen(mode);

    return Window(mode->width, mode->height, title, monitor);
}

RenderContext::RenderContext() {
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        spdlog::error("GLEW is not initialized!\n");
    }

    print_opengl_info();

#if defined(_DEBUG)
    if (glDebugMessageCallback)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugCallback, nullptr);

        GLuint unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
    }
    else
    {
        spdlog::error("glDebugMessageCallback unavailable...\n");
    }
#endif
}
RenderContext::~RenderContext() {
}
