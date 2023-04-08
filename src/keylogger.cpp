#include "keylogger.h"

#include "VKRenderer.h"

#include <set>

std::map<GLFW_KEY, KEY_ACTION> key_action_map;
std::set<KEY_ACTION*> pressed_actions_set;

bool is_key_pressed(GLFW_KEY key) {
    return key_action_map[key] == KEY_PRESSED;
}

bool is_key_held(GLFW_KEY key) {
    return is_key_pressed(key) || key_action_map[key] == KEY_HELD;
}

bool is_key_released(GLFW_KEY key) {
    return key_action_map[key] == KEY_RELEASED;
}

void update_keylogger() { 
    for(KEY_ACTION* action : pressed_actions_set) {
        if(*action == KEY_PRESSED) {
            *action = KEY_HELD;
        }
    }
    pressed_actions_set.clear();
}

void setup_keylogger(VKRenderer& renderer) {
    /*
    create a key callback function that will log the keys
    */
    std::function<void(GLFWwindow*, int, int, int, int)> keyCallback = [&](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if(action == GLFW_PRESS) {
            key_action_map[key] = KEY_PRESSED;
            pressed_actions_set.insert(&key_action_map[key]);
        }

        if(action == GLFW_RELEASE) {
            key_action_map[key] = KEY_RELEASED;
        }
    };

    //sets the key callback function to log keys
    renderer.getEngine()->getDisplay()->setKeyCallback(keyCallback);
}