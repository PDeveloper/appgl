#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace ImGui
{

bool SliderDouble(const char* label, double* v, double v_min, double v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

void CheckboxAlignRight(const char* label);
bool CheckboxRight(const char* label, bool* v, bool align_right = false);

} // namespace ImGui
