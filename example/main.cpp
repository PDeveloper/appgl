#include <iostream>

#include "appgl/display.hpp"
#include "appgl/gui_events.hpp"
#include "appgl/platform.hpp"
#include "appgl/logging.hpp"

#include "appgl/ui/performance.hpp"

struct Application {
    appgl::Window& window;

    size_t count = 0;

    Application(appgl::Window& window) : window(window) {
        appgl::logging::create("Example");
        appgl::set_window_callbacks(window, *this);
    }

    void on_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            count++;
            appgl::logger("Example")->info("A key is pressed {}", count);
        }
    }
};

int main() {
    appgl::Platform platform;
    auto window = appgl::Window::create("Hello World", 1280, 720);
    auto display = appgl::Display::Initializer(window);

    Application application(window);

    auto gui = appgl::Gui(window);

    Performance performance;

    char buffer[256] = {0};

    while (!window.should_close()) {
        platform.poll_events();
        
        gui.frame();

        // Draw a fullscreen ImGui window with some text
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Fullscreen Window", nullptr);
        ImGui::Text("Hello 123");
        ImGui::InputText("Text", buffer, 256);
        if (ImGui::Button("Button")) {
            spdlog::info("button is pressed");
        }
        ImGui::End();

        performance.render();

        gui.render();

        window.swap();
    }

    return EXIT_SUCCESS;
}
