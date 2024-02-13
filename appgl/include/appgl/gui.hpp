#pragma once

#include "window.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include <string>

namespace appgl {

struct Gui {
    Gui();
    ~Gui();

    void init(Window& window, bool install_callbacks = true, const std::string& glsl_version = "#version 330");
    void callbacks(Window& window);
    
    void frame();
    void render(Window& window);
};

} // namespace appgl
