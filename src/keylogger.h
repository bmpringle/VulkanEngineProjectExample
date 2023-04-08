#ifndef KEYLOGGER_H
#define KEYLOGGER_H

class VKRenderer;

#include <map>

//KEY_ACTION enum used instead of GLFW_PRESS/GLFW_RELEASE because there is no GLFW_HELD value
enum KEY_ACTION {
    KEY_RELEASED, //first in the enum because KEY_ACTION(0) is the default init value for std::map
    KEY_PRESSED,
    KEY_HELD
};

//used to make clear that the function is expecting the GLFW api's GLFW_KEY_X as an input.
typedef int GLFW_KEY;

//returns true if the key was pressed this frame
bool is_key_pressed(GLFW_KEY key); 

//returns true if the key was not pressed this frame but has not yet been released
bool is_key_held(GLFW_KEY key);

//returns true if the key has not been pressed since the last time it was released
bool is_key_released(GLFW_KEY key); 

//call right before glfwPollEvents
void update_keylogger();

//call before using the key logger
void setup_keylogger(VKRenderer& renderer);

#endif