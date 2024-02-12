#pragma once

#include "appgl/gui.hpp"
#include "appgl/utils/stats.hpp"

#include <algorithm>
#include <chrono>
#include <vector>

namespace appgl::ui {

// Class to track the fps and memory usage of the application
struct Performance {
    size_t fps_buffer_size = 100;

	size_t frame_count = 0;
    std::chrono::high_resolution_clock::time_point frame_time = std::chrono::high_resolution_clock::now();

	double fps = 0.0;

    std::vector<float> fps_buffer;

	void render() {
        frame_count++;

        auto now = std::chrono::high_resolution_clock::now();
        auto duration = frame_time - now;

        if (duration >= std::chrono::seconds(1)) {
            frame_time = now;
            fps = static_cast<double>(frame_count) / (std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() / 1000000000.0);

            fps_buffer.emplace_back(static_cast<float>(fps));
            if (fps_buffer.size() > fps_buffer_size) fps_buffer.erase(fps_buffer.begin());

            frame_count = 0;
        }

        float max_fps = !fps_buffer.empty() ? *std::max_element(fps_buffer.begin(), fps_buffer.end()) : 0.0f;

        size_t memory_usage = utils::get_memory_usage() / (1024 * 1024);
        size_t gpu_memory_usage = utils::get_gpu_memory_usage() / (1024 * 1024);
        auto stats_str = fmt::format("{:.1f}fps | RAM: {}MB | GPU: {}MB", fps, memory_usage, gpu_memory_usage);
        ImGui::Text("%s", stats_str.c_str());

        auto width = ImGui::GetContentRegionAvail().x;
        ImGui::PlotLines("", fps_buffer.data(), (int)fps_buffer.size(), 0, NULL, 0.0f, max_fps, ImVec2(width, 80));
	}
};

} // namespace appgl::ui
