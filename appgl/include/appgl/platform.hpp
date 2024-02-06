#pragma once

#include <span>

#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

namespace appgl {

class Platform {
public:
    Platform();
    ~Platform();

    void poll_events();
    std::span<GLFWmonitor*> monitors();
};

} // namespace appgl
