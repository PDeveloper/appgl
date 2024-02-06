#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <type_traits>

namespace appgl {

template<typename T>
concept HasWindowFocusCallback = requires(T a, GLFWwindow* window, int focused) {
    { a.on_window_focus(window, focused) } -> std::same_as<void>;
};

template<typename T>
concept HasCursorEnterCallback = requires(T a, GLFWwindow* window, int entered) {
    { a.on_cursor_enter(window, entered) } -> std::same_as<void>;
};

template<typename T>
concept HasCursorPosCallback = requires(T a, GLFWwindow* window, double xpos, double ypos) {
    { a.on_cursor_pos(window, xpos, ypos) } -> std::same_as<void>;
};

template<typename T>
concept HasMouseButtonCallback = requires(T a, GLFWwindow* window, int button, int action, int mods) {
    { a.on_mouse_button(window, button, action, mods) } -> std::same_as<void>;
};

template<typename T>
concept HasScrollCallback = requires(T a, GLFWwindow* window, double xoffset, double yoffset) {
    { a.on_scroll(window, xoffset, yoffset) } -> std::same_as<void>;
};

template<typename T>
concept HasKeyCallback = requires(T a, GLFWwindow* window, int key, int scancode, int action, int mods) {
    { a.on_key(window, key, scancode, action, mods) } -> std::same_as<void>;
};

template<typename T>
concept HasCharCallback = requires(T a, GLFWwindow* window, unsigned int c) {
    { a.on_char(window, c) } -> std::same_as<void>;
};

template<HasWindowFocusCallback THandler>
void appgl_WindowFocusCallback(GLFWwindow* window, int focused) {
    THandler* handler = static_cast<THandler*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->on_window_focus(window, focused);
    }
}

template<HasCursorEnterCallback THandler>
void appgl_CursorEnterCallback(GLFWwindow* window, int entered) {
    THandler* handler = static_cast<THandler*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->on_cursor_enter(window, entered);
    }
}

template<HasCursorPosCallback THandler>
void appgl_CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    THandler* handler = static_cast<THandler*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->on_cursor_pos(window, xpos, ypos);
    }
}

template<HasMouseButtonCallback THandler>
void appgl_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    THandler* handler = static_cast<THandler*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->on_mouse_button(window, button, action, mods);
    }
}

template<HasScrollCallback THandler>
void appgl_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    THandler* handler = static_cast<THandler*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->on_scroll(window, xoffset, yoffset);
    }
}

template<HasKeyCallback THandler>
void appgl_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (ImGui::GetIO().WantCaptureKeyboard) {
        return;
    }
    THandler* handler = static_cast<THandler*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->on_key(window, key, scancode, action, mods);
    }
}

template<HasCharCallback THandler>
void appgl_CharCallback(GLFWwindow* window, unsigned int c) {
    if (ImGui::GetIO().WantTextInput) {
        return;
    }
    THandler* handler = static_cast<THandler*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->on_char(window, c);
    }
}

template<typename THandler>
struct MonitorHandler {
    static THandler* monitor_handler_ptr;
};

template<typename THandler>
void appgl_MonitorCallback(GLFWmonitor* monitor, int event) {
    THandler* handler = MonitorHandler<THandler>::monitor_handler_ptr;
    if (handler) {
        handler->on_monitor(monitor, event);
    }
}

template<typename THandler>
void set_monitor_callback(THandler* handler) {
    MonitorHandler<THandler>::monitor_handler_ptr = handler;
    glfwSetMonitorCallback(appgl_MonitorCallback<THandler>);
}

template<typename THandler>
void set_window_callbacks(GLFWwindow* window, THandler& handler) {
    glfwSetWindowUserPointer(window, &handler);
    if constexpr (HasWindowFocusCallback<THandler>) glfwSetWindowFocusCallback(window, appgl_WindowFocusCallback<THandler>);
    if constexpr (HasCursorEnterCallback<THandler>) glfwSetCursorEnterCallback(window, appgl_CursorEnterCallback<THandler>);
    if constexpr (HasCursorPosCallback<THandler>) glfwSetCursorPosCallback(window, appgl_CursorPosCallback<THandler>);
    if constexpr (HasMouseButtonCallback<THandler>) glfwSetMouseButtonCallback(window, appgl_MouseButtonCallback<THandler>);
    if constexpr (HasScrollCallback<THandler>) glfwSetScrollCallback(window, appgl_ScrollCallback<THandler>);
    if constexpr (HasKeyCallback<THandler>) glfwSetKeyCallback(window, appgl_KeyCallback<THandler>);
    if constexpr (HasCharCallback<THandler>) glfwSetCharCallback(window, appgl_CharCallback<THandler>);
}

} // namespace appgl
