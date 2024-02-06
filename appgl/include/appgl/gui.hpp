#pragma once

#include "window.hpp"
#include "imgui.h"

#include <string>

namespace appgl {

struct Gui {
    Window& window;

    Gui(Window& window, bool install_callbacks = true, const std::string& glsl_version = "#version 330");
    ~Gui();

    void init_callbacks();

    void frame();
    void render();
};

} // namespace appgl
