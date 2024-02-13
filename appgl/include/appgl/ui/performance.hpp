#pragma once

#include <algorithm>
#include <chrono>
#include <vector>

#include <spdlog/spdlog.h>

#include "appgl/gui.hpp"
#include "appgl/utils/stats.hpp"

namespace appgl::ui {

// Class to track the fps and memory usage of the application
struct Performance {
    std::chrono::high_resolution_clock::time_point frame_time = std::chrono::high_resolution_clock::now();

    size_t buffer_size = 100;
    std::vector<float> buffer;

	double fps = 0.0;
	size_t frame_count = 0;

	void render() {
        frame_count++;

        auto now = std::chrono::high_resolution_clock::now();
        auto duration = frame_time - now;

        if (duration >= std::chrono::seconds(1)) {
            frame_time = now;
            fps = static_cast<double>(frame_count) / (std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() / 1000000000.0);

            buffer.emplace_back(static_cast<float>(fps));
            if (buffer.size() > buffer_size) buffer.erase(buffer.begin());

            frame_count = 0;
        }

        float max_fps = !buffer.empty() ? *std::max_element(buffer.begin(), buffer.end()) : 0.0f;

        size_t memory_usage = utils::get_memory_usage() / (1024 * 1024);
        size_t gpu_memory_usage = utils::get_gpu_memory_usage() / (1024 * 1024);
        auto stats_str = fmt::format("{:.1f}fps | RAM: {}MB | GPU: {}MB", fps, memory_usage, gpu_memory_usage);
        ImGui::Text("%s", stats_str.c_str());

        auto width = ImGui::GetContentRegionAvail().x;
        ImGui::PlotLines("", buffer.data(), (int)buffer.size(), 0, NULL, 0.0f, max_fps, ImVec2(width, 80));
	}
};

} // namespace appgl::ui
