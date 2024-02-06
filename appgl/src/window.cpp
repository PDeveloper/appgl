#include "appgl/window.hpp"

#include "spdlog/spdlog.h"

#include <unordered_map>

namespace appgl {

void window_hints_gl(int gl_major = 3, int gl_minor = 3) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void window_hints_fullscreen(const GLFWvidmode* mode) {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
}

Window::Window(int width, int height, const char* title, void* user_data, GLFWmonitor* monitor, GLFWwindow* share) :
    window(glfwCreateWindow(width, height, title, monitor, share), glfwDestroyWindow),
    user_data(user_data)
{
    if (!window) {
        spdlog::error("Failed to create GLFW window");
    } else {
        glfwSetWindowUserPointer(window.get(), &user_data);
    }
}
void Window::make_current() {
    glfwMakeContextCurrent(*this);
}
bool Window::should_close() {
    return glfwWindowShouldClose(*this);
}
void Window::swap() {
    glfwSwapBuffers(*this);
}
std::pair<int, int> Window::framebuffer_size() const {
    int width, height;
    glfwGetFramebufferSize(*this, &width, &height);
    return {width, height};
}
void Window::set_user_data(void* user_data) {
    this->user_data = user_data;
    glfwSetWindowUserPointer(*this, user_data);
}
Window::operator GLFWwindow* () const {
    return window.get();
}
Window Window::create(const char* title, int width, int height, void* user_data) {
    window_hints_gl();
    return Window(width, height, title, user_data);
}
Window Window::create_fullscreen(const char* title, void* user_data, GLFWmonitor* monitor) {
    if (!monitor) monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    window_hints_gl();
    window_hints_fullscreen(mode);

    return Window(mode->width, mode->height, title, user_data, monitor);
}

} // namespace appgl
