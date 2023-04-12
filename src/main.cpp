#include "VKRenderer.h"
#include "keylogger.h"
#include <glm/ext/matrix_transform.hpp>
#include "ModelLoader.h"
#include "collision.h"
#include <unordered_map>

double current_mouse_x = 0;
double current_mouse_y = 0;
const double mouse_sensitivity = 0.04;
double movement_speed = 4.0 / 60.0;
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

std::unordered_map<std::string, game_object_type> object_data_map;

const std::string game_models[] = {"floor_tile.obj"};
const std::string game_model_ids[] = {"floor1x1"};
const int GAME_MODEL_COUNT = 1;

void world_render_setup(VKRenderer& renderer) {
  /*
    load all game models
  */

  std::vector<std::string> textures_to_load;

  for(int i = 0; i < GAME_MODEL_COUNT; ++i) {
    object_data_map[game_model_ids[i]] = game_object_type("assets/", game_models[i], game_model_ids[i]);
    auto [id, object_type] = *(object_data_map.find(game_model_ids[i]));

    if(std::find(textures_to_load.cbegin(), textures_to_load.cend(), object_type.get_texture()) == textures_to_load.cend()) {
      textures_to_load.push_back(object_type.get_texture());
    }
  }

  /*
    Load a texture array with id "game-textures" with all game textures. this works because they are all the same size.
  */
  renderer.loadTextureArray("game-textures", textures_to_load);

  //set the array with id "game-textures" to be the currently in use texture array
  renderer.setCurrentTextureArray("game-textures");

  //go through all models and set their UV values properly

  for(auto [id, object_type] : object_data_map) {
    for(Vertex& v : object_type.get_mutable_vertices()) { 
      v.texCoord[2] = renderer.getTextureArrayID("game-textures", object_type.get_texture());
    }

    //setup imported model and instances
    renderer.setModel(id, object_type.get_vertices());
  }

  //set the camera's starting position to be (2, 2, 0)
  renderer.getCameraPosition() = glm::vec3(0, 2, 0);
} 

void do_movement_checks(VKRenderer& renderer) {
  glm::vec3 base_movement_vector = (float)movement_speed * getHorizontalCameraVector(renderer);

  //move in the horizontal direction you are looking when W is held
  if(is_key_held(GLFW_KEY_W)) {
    renderer.getCameraPosition() += base_movement_vector;
  }

  //move in the opposite of the horizontal direction you are looking when S is held
  if(is_key_held(GLFW_KEY_S)) {
    renderer.getCameraPosition() += -base_movement_vector;
  }

  //move 90 degrees to the left of the horizontal direction you are looking when A is held
  if(is_key_held(GLFW_KEY_A)) {
    glm::vec3 rotated_move = base_movement_vector;
    std::swap(rotated_move.x, rotated_move.z);
    rotated_move.z = -rotated_move.z;
    renderer.getCameraPosition() += rotated_move;
  }

  //move 90 degrees to the right of the horizontal direction you are looking when D is held
  if(is_key_held(GLFW_KEY_D)) {
    glm::vec3 rotated_move = base_movement_vector;
    std::swap(rotated_move.x, rotated_move.z);
    rotated_move.x = -rotated_move.x;
    renderer.getCameraPosition() += rotated_move;
  }
}

void do_cursor_mode_check(VKRenderer& renderer) {
  auto display = renderer.getEngine()->getDisplay();

  //toggle cursor with the G key
  if(is_key_pressed(GLFW_KEY_G)) {
    auto mode = GLFW_CURSOR_NORMAL;
    if(glfwGetInputMode(display->getInternalWindow(), GLFW_CURSOR) == mode) {
      mode = GLFW_CURSOR_DISABLED;
      is_mouse_captured = true;
    }else {
      is_mouse_captured = false;
    }
    
    glfwSetInputMode(display->getInternalWindow(), GLFW_CURSOR, mode);
  }
}

void do_esc_key_check(VKRenderer& renderer) {
  auto display = renderer.getEngine()->getDisplay();

  //set the window to clsoe if the escape key is pressed
  if(is_key_pressed(GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(display->getInternalWindow(), 1);
  }
}

auto previous_tick_time = std::chrono::high_resolution_clock::now();
const int LOGIC_TICKS_PER_SECOND = 60;

int calculate_missed_ticks() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - previous_tick_time).count() / 1000.0 * LOGIC_TICKS_PER_SECOND;
}

int main() {
  //init renderer
  VKRenderer renderer = VKRenderer();

  //setup world render
  world_render_setup(renderer);

  std::vector<InstanceData> floor_data;

  //create floor
  for(int x = -49; x < 50; ++x) {
    for(int z = -49; z < 50; ++z) {
      floor_data.push_back(InstanceData({{x * 10, 0, z * 10}}));
    }
  }

  renderer.addInstancesToModel("floor1x1", "s1", floor_data);

  //set window callbacks
  createWindowCallbacks(renderer);

  auto display = renderer.getEngine()->getDisplay();

  previous_tick_time = std::chrono::high_resolution_clock::now();

  //run the main loop while the window should remain open
  while(!display->shouldWindowClose()) {
    int missed_ticks = calculate_missed_ticks();

    while(missed_ticks > 0) {
      previous_tick_time = std::chrono::high_resolution_clock::now();

      for(int i = 0; i < missed_ticks; ++i) {
        do_movement_checks(renderer);
      }

      missed_ticks = calculate_missed_ticks();
    }

    do_cursor_mode_check(renderer);

    do_esc_key_check(renderer);

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