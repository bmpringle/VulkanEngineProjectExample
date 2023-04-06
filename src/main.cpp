#include "VKRenderer.h"

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

  //set the camera's position to be (2, 2, 0)
  renderer.getCameraPosition() = glm::vec3(2, 2, 0);

  /*
    create a key callback function that will check if the escape 
    key is pressed, and mark the window to be closed if it is
  */
  std::function<void(GLFWwindow*, int, int, int, int)> keyCallback = [&](GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  };

  //set a key callback for the window
  renderer.getEngine()->getDisplay()->setKeyCallback(keyCallback);

  //by default the renderer disables the cursor, so re-enable it
  glfwSetInputMode(renderer.getEngine()->getDisplay()->getInternalWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  //run the main loop while the window should remain open
  while(!renderer.getEngine()->getDisplay()->shouldWindowClose()) {
    //record command buffers
    renderer.recordCommandBuffers();

    //render the frame
    renderer.renderFrame();

    //poll for window events
    glfwPollEvents();
  }

  return 0;
}
