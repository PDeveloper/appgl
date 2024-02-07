#include "appgl/platform.hpp"

#include <nfd.hpp>

namespace appgl {

Platform::Platform() {
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW");
        std::exit(EXIT_FAILURE);
    }
    NFD::Init();
}
Platform::~Platform() {
    NFD::Quit();
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
