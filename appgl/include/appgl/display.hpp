#pragma once

#include "gl.hpp"
#include "gui.hpp"
#include "window.hpp"

namespace appgl {

struct Display {
    struct Initializer {
        Initializer(Window& window);
    };

    Window window;
    Initializer initializer;
    Gui gui;

    Display(Window&& window);
    ~Display() = default;
};

}
