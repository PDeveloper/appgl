#include <iostream>

#include "appgl/gui.hpp"
#include "appgl/platform.hpp"

#include "appgl/ui/performance.hpp"

int main() {
    Platform platform;

    auto window = Window::create(1280, 720, "Hello World");
    window.make_current();

    RenderContext render;

    Gui gui(window);

    Performance performance;

    while (!window.should_close()) {
        platform.poll_events();
        
        gui.frame();

        // Draw a fullscreen ImGui window with some text
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Fullscreen Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::Text("Hello 123");
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
