#include "appgl/display.hpp"

#include "spdlog/spdlog.h"

namespace appgl {

Display::Initializer::Initializer(Window& window) {
    window.make_current();
    gl_init();
}

Display::Display(Window&& window) :
    window(std::move(window)),
    initializer(this->window),
    gui(this->window)
{
}

} // namespace appgl
