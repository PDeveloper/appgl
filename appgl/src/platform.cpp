#include "appgl/platform.hpp"

namespace appgl {

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

} // namespace appgl
