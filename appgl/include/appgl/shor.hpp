#include <GLFW/glfw3.h>

#include <functional>
#include <string>
#include <vector>

namespace shor {

class shor {
public:
    struct shortcut {
        int _key;
        int _mods;

        std::function<void()> _callback;

        shortcut& ctrl() {
            _mods |= GLFW_MOD_CONTROL;
            return *this;
        }

        shortcut& shift() {
            _mods |= GLFW_MOD_SHIFT;
            return *this;
        }

        shortcut& alt() {
            _mods |= GLFW_MOD_ALT;
            return *this;
        }

        shortcut& key(int key) {
            _key = key;
            return *this;
        }

        shortcut& callback(std::function<void()> callback) {
            _callback = callback;
            return *this;
        }

        shortcut& action(int key, std::function<void()> callback) {
            _key = key;
            _callback = callback;
            return *this;
        }

        void check(int key, int mods) {
            if (key == _key && mods & _mods == _mods) {
                _callback();
            }
        }

        std::string string() {
            std::string str = glfwGetKeyName(_key, 0);
            if (_mods & GLFW_MOD_SHIFT) {
                str = "Shift+" + str;
            }
            if (_mods & GLFW_MOD_CONTROL) {
                str = "Ctrl+" + str;
            }
            return str;
        }
    };

    std::vector<shortcut> shortcuts;

    shortcut& ctrl() {
        return shortcuts.emplace_back(0, GLFW_MOD_CONTROL);
    }

    shortcut& shift() {
        return shortcuts.emplace_back(0, GLFW_MOD_SHIFT);
    }

    shortcut& alt() {
        return shortcuts.emplace_back(0, GLFW_MOD_ALT);
    }

    void check(int key, int mods) {
        for (auto& shortcut : shortcuts) {
            shortcut.check(key, mods);
        }
    }
};

}
