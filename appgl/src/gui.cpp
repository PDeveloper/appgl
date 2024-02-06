#include "appgl/gui.hpp"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace appgl {

Gui::Gui(Window& window, bool install_callbacks, const std::string& glsl_version) :
    window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, install_callbacks);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}
Gui::~Gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::init_callbacks() {
    ImGui_ImplGlfw_InstallCallbacks(window);
}
void Gui::frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::render() {
    ImGui::Render();

    auto [display_width, display_height] = window.framebuffer_size();

    glViewport(0, 0, display_width, display_height);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace appgl
