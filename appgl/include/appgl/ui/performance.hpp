#pragma once

#include "../gui.hpp"

#include <algorithm>
#include <vector>

// Class to track the fps and memory usage of the application
struct Performance {
    bool show = true;

	size_t frame_count = 0;
	double last_time = 0.0;
	double fps = 0.0;

    float max_fps = 0.0f;
    std::vector<float> fps_buffer;

	void render() {
        double current_time = glfwGetTime();
        frame_count++;

        auto delta = current_time - last_time;
        if (delta >= 1.0) {
            last_time = current_time;
            fps = static_cast<double>(frame_count) / delta;
            fps_buffer.emplace_back(static_cast<float>(fps));
            if (fps_buffer.size() > 100) fps_buffer.erase(fps_buffer.begin());
            max_fps = *std::max_element(fps_buffer.begin(), fps_buffer.end());

            frame_count = 0;
        }

        if (show) {
            ImGui::Begin("Performance", &show, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("FPS: %.1f", fps);
            ImGui::PlotLines("", fps_buffer.data(), (int)fps_buffer.size(), 0, NULL, 0.0f, max_fps, ImVec2(0, 80));

            ImGui::End();
        }
	}
};
