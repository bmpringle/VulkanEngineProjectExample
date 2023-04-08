#include "VKRenderer.h"
#include "keylogger.h"
#include <glm/ext/matrix_transform.hpp>

static std::vector<Vertex> cube = {
  {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1, 0, 0}}, //front
  {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0, 1, 0}},
  {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1, 1, 0}},

  {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1, 0, 0}},
  {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0, 0, 0}},
  {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0, 1, 0}},   

  {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0, 1, 0}}, //back
  {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1, 1, 0}},
  {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0, 0, 0}},

  {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1, 1, 0}},  
  {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1, 0, 0}},
  {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0, 0, 0}},

  {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0, 0, 0}}, //left
  {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1, 1, 0}},
  {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1, 0, 0}},

  {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0, 0, 0}},
  {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0, 1, 0}},
  {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1, 1, 0}},

  {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0, 0, 0}}, //right
  {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0, 1, 0}},
  {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1, 0, 0}},

  {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0, 1, 0}},
  {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1, 1, 0}},
  {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1, 0, 0}},
  
  {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0, 0, 0}}, //top
  {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0, 1, 0}},
  {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1, 0, 0}},

  {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0, 1, 0}},
  {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1, 1, 0}},
  {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1, 0, 0}},

  {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0, 0, 0}}, //bottom
  {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1, 1, 0}},
  {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1, 0, 0}},

  {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0, 0, 0}},
  {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0, 1, 0}},
  {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1, 1, 0}},
};

double current_mouse_x = 0;
double current_mouse_y = 0;
const double mouse_sensitivity = 0.04;
double movement_speed = 1.0 / 60.0;
bool is_mouse_captured = true;

//get X and Z values of the camera vector (normalized)
glm::vec3 getHorizontalCameraVector(VKRenderer& renderer) { 
  double angle = (renderer.getXRotation() + 90.0);

  if(angle > 360) {
    angle -= 360;
  }

  angle *= M_PI / 180.0;

  return glm::vec3(-cos(angle), 0, sin(angle));
}

//setup key logger and mouse callback
void createWindowCallbacks(VKRenderer& renderer) {

  //setup the key logger
  setup_keylogger(renderer);
  
  /*
    create a mouse callback function that will change the current mouse position and rotate the camera
  */
  std::function<void(GLFWwindow*, double, double)> mouseCallback = [&](GLFWwindow* window, double x, double y) {
    if(!is_mouse_captured) return; //only use mouse if it is captured by screen

    double x_diff_adjusted = (x - current_mouse_x) * mouse_sensitivity;
    double y_diff_adjusted = (y - current_mouse_y) * mouse_sensitivity;

    renderer.getXRotation() -= x_diff_adjusted;
    renderer.getYRotation() += y_diff_adjusted;

    if(renderer.getYRotation() > 90) {
      renderer.getYRotation() = 90;
    }

    if(renderer.getYRotation() < -90) {
      renderer.getYRotation() = -90;
    }

    while(renderer.getXRotation() >= 360.0) {
      renderer.getXRotation() -= 360.0;
    }

    while(renderer.getXRotation() < 0) {
      renderer.getXRotation() += 360.0;
    }
    
    current_mouse_x = x;
    current_mouse_y = y;
  };

  //set callbacks for the window
  renderer.getEngine()->getDisplay()->setCursorPosCallback(mouseCallback);
}

int main() {
  //init renderer
  VKRenderer renderer = VKRenderer();

  //create a cube instance
  std::vector<InstanceData> data = {InstanceData({{0, 0, 3}})};

  //set cube model to the "mdl" id
  renderer.setModel("mdl", cube);

  //add instances in data vector to the "s1" instance set
  renderer.addInstancesToModel("mdl", "s1", data);

  /*
    load a texture array with id "1" with just the dirt image. Note that we don't need to 
    use VKRenderer::getTextureArrayID to get the texture id that should be in the cube model data because 
    with just one texture in the array, the texture id will always be 0.
  */
  renderer.loadTextureArray("1", {"assets/dirt.png"});

  //set the array with id "1" to be the currently in use texture array
  renderer.setCurrentTextureArray("1");

  //set the camera's starting position to be (2, 2, 0)
  renderer.getCameraPosition() = glm::vec3(2, 2, 0);

  //set window callbacks
  createWindowCallbacks(renderer);

  //run the main loop while the window should remain open
  while(!renderer.getEngine()->getDisplay()->shouldWindowClose()) {
    //move in the horizontal direction you are looking when W is held
    if(is_key_held(GLFW_KEY_W)) {
      renderer.getCameraPosition() += (float)movement_speed * getHorizontalCameraVector(renderer);
    }

    //move in the opposite of the horizontal direction you are looking when S is held
    if(is_key_held(GLFW_KEY_S)) {
      renderer.getCameraPosition() += -(float)movement_speed * getHorizontalCameraVector(renderer);
    }

    //move 90 degrees to the left of the horizontal direction you are looking when A is held
    if(is_key_held(GLFW_KEY_A)) {
      glm::vec3 rotated_move = getHorizontalCameraVector(renderer);
      std::swap(rotated_move.x, rotated_move.z);
      rotated_move.z = -rotated_move.z;
      renderer.getCameraPosition() += (float)movement_speed * rotated_move;
    }

    //move 90 degrees to the right of the horizontal direction you are looking when D is held
    if(is_key_held(GLFW_KEY_D)) {
      glm::vec3 rotated_move = getHorizontalCameraVector(renderer);
      std::swap(rotated_move.x, rotated_move.z);
      rotated_move.x = -rotated_move.x;
      renderer.getCameraPosition() += (float)movement_speed * rotated_move;
    }

    //toggle cursor with the G key
    if(is_key_pressed(GLFW_KEY_G)) {
      auto mode = GLFW_CURSOR_NORMAL;
      if(glfwGetInputMode(renderer.getEngine()->getDisplay()->getInternalWindow(), GLFW_CURSOR) == mode) {
        mode = GLFW_CURSOR_DISABLED;
        is_mouse_captured = true;
      }else {
        is_mouse_captured = false;
      }
      
      glfwSetInputMode(renderer.getEngine()->getDisplay()->getInternalWindow(), GLFW_CURSOR, mode);
    }

    //set the window to clsoe if the escape key is pressed
    if(is_key_pressed(GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(renderer.getEngine()->getDisplay()->getInternalWindow(), 1);
    }

    //record command buffers
    renderer.recordCommandBuffers();

    //render the frame
    renderer.renderFrame();

    /*
      IMPORTANT: key logger must be updated right BEFORE polling for events. 
      it will not work properly if called before statements that use its functionality 
      and will not work properly if called right after
    */
    update_keylogger();

    //poll for window events
    glfwPollEvents();
  }

  return 0;
}
