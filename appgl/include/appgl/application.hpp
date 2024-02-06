#pragma once

#include "platform.hpp"
#include "display.hpp"

namespace appgl {

template<typename TApplication>
class WindowApplication : public TApplication {
public:
    appgl::Platform platform;
    appgl::Window window;
    appgl::Display::Initializer initializer;
    appgl::Gui gui;
    TApplication application;

    WindowApplication(int width, int height, const char* title) :
        display(appgl::Window::create(width, height, title))
    {
GLFWwindowfocusfun      PrevUserCallbackWindowFocus;
GLFWcursorposfun        PrevUserCallbackCursorPos;
GLFWcursorenterfun      PrevUserCallbackCursorEnter;
GLFWmousebuttonfun      PrevUserCallbackMousebutton;
GLFWscrollfun           PrevUserCallbackScroll;
GLFWkeyfun              PrevUserCallbackKey;
GLFWcharfun             PrevUserCallbackChar;
GLFWmonitorfun          PrevUserCallbackMonitor;
        glfwSetWindowFocusCallback(window, ImGui_ImplGlfw_WindowFocusCallback);
        glfwSetCursorEnterCallback(window, ImGui_ImplGlfw_CursorEnterCallback);
        glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
        glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
        glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
        glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
        glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
        glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);
    }
    ~WindowApplication() = default;

    void run() {
        while (!display.window.should_close()) {
            platform.poll_events();
            
            display.gui.frame();

            display.gui.render();
            display.window.swap();
        }
    }
};

} // namespace appgl
