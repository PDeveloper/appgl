#include "appgl/platform.hpp"

#include <nfd.hpp>

#include <cstdlib> // For getenv()
#include <string>
#if defined(_WIN32)
#include <windows.h>
#include <shlobj.h> // For SHGetFolderPath()
#elif defined(__APPLE__)
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

namespace appgl {

std::string get_app_data_directory() {
    std::string path;
#if defined(_WIN32)
    // Windows
    char appDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetKnownFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        path = appDataPath;
    }
#elif defined(__APPLE__)
    // macOS
    const char* homeDir = getenv("HOME");
    if (!homeDir) {
        struct passwd* pwd = getpwuid(getuid());
        if (pwd) {
            homeDir = pwd->pw_dir;
        }
    }
    if (homeDir) {
        path = std::string(homeDir) + "/Library/Application Support";
    }
#else
    // Linux and other Unix-like systems
    const char* xdgDataHome = getenv("XDG_DATA_HOME");
    if (xdgDataHome) {
        path = xdgDataHome;
    } else {
        const char* homeDir = getenv("HOME");
        if (homeDir) {
            path = std::string(homeDir) + "/.local/share";
        }
    }
#endif
    return path;
}

Platform::Platform() {
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW");
        std::exit(EXIT_FAILURE);
    }
    NFD::Init();
}
Platform::~Platform() {
    NFD::Quit();
    glfwTerminate();
}
void Platform::poll_events() {
    glfwPollEvents();
}
std::span<GLFWmonitor*> Platform::monitors() {
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    return std::span<GLFWmonitor*>(monitors, count);
}

} // namespace appgl
