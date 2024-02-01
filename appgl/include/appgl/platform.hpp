#pragma once

#include <span>

#include "gl.hpp"
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

struct Platform {
    Platform();
    ~Platform();

    void poll_events();
    std::span<GLFWmonitor*> monitors();
};

class Window {
public:
    Window(int width, int height, const char* title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
    ~Window();
    void make_current();
    bool should_close();
    void swap();

    operator GLFWwindow* () const;
    
    static Window create(int width, int height, const char* title);
    static Window create_fullscreen(const char* title, GLFWmonitor* monitor = nullptr);

private:
    GLFWwindow* window;
};

struct RenderContext {
    RenderContext();
    ~RenderContext();
};
