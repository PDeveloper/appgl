#include <iostream>

#include <nfd.hpp>

#include "appgl/display.hpp"
#include "appgl/gui_events.hpp"
#include "appgl/platform.hpp"
#include "appgl/logging.hpp"

#include "appgl/ui/imgui_theme.hpp"
#include "appgl/ui/performance.hpp"

struct Application {
    appgl::Window& window;

    size_t count = 0;

    Application(appgl::Window& window) : window(window) {
        appgl::logging::create("Example");
        appgl::set_window_callbacks(window, *this);
        
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 16.0f);
        if (!font) {
            appgl::logger("Example")->error("Failed to load \"fonts/Roboto-Regular.ttf\"");
        }
        ImGuiTheme::Theme theme = ImGuiTheme::Theme::MaterialFlat;
        ImGuiTheme::ApplyTheme(theme);
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
    appgl::Gui gui;

    Application application(window);

    gui.init(window);

    appgl::Texture carp_image;
    carp_image.load("example/carp_64x64.png");

    appgl::ui::Performance performance;

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
            appgl::logger("Example")->info("button is pressed");
            
            NFD::UniquePath outPath;
            nfdfilteritem_t filterItem[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
            nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 2, NULL);
            if (result == NFD_OKAY)
            {
                appgl::logger("Example")->info("User selected file: {}", outPath.get());
            }
            else if (result == NFD_CANCEL)
            {
                appgl::logger("Example")->info("User pressed cancel");
            }
            else 
            {
                appgl::logger("Example")->error("Error: {}", NFD::GetError());
            }
        }
        ImGui::Image((ImTextureID)carp_image.id, ImVec2(64, 64));
        ImGui::End();

        performance.render();

        gui.render(window);
        window.swap();
    }

    return EXIT_SUCCESS;
}
