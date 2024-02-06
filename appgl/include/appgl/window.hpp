#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <tuple>

namespace appgl {

class Window {
public:
    using GLFWwindowDeleter = void(*)(GLFWwindow*);

    Window(int width, int height, const char* title, void* user_data = nullptr, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
    Window(Window&&) = default;
    ~Window() = default;

    void make_current();
    bool should_close();
    void swap();
    std::pair<int, int> framebuffer_size() const;
    void set_user_data(void* user_data);

    operator GLFWwindow* () const;
    
    static Window create(const char* title, int width, int height, void* user_data = nullptr);
    static Window create_fullscreen(const char* title, void* user_data = nullptr, GLFWmonitor* monitor = nullptr);

private:
    std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window;
    void* user_data;
};

}
