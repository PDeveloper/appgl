#pragma once

#include "platform.hpp"

#include "imgui.h"

struct Gui {
    Window& window;

    Gui(Window& window);
    ~Gui();

    void frame();
    void render();
};
