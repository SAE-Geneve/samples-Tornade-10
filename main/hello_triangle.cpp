#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#include "engine.h"
#include "file_utility.h"
#include "scene.h"
#include "camera.h"
#include "shader.h"

namespace gpr5300 {
class HelloTriangle final : public Scene {
 public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  void OnEvent(const SDL_Event &event) override;
  void UpdateCamera(const float dt) override;

  float elapsed_time_ = 0;

 private:
  Shader program_ = {}; // It's the OurShader
  Shader cubemap_shader = {};
  GLuint vertexShader_ = 0;
  GLuint fragmentShader_ = 0;
  GLuint cubemap_vao_ = 0;
  GLuint map_textureID = 0;

  unsigned int cubeVAO, cubeVBO;
  unsigned int skyboxVAO, skyboxVBO;
  unsigned int cube_texture_;

  Camera camera_;

  std::array<std::string, 6> faces
  {
    "data/shaders/hello_cube_map/skybox/right.jpg",
    "data/shaders/hello_cube_map/skybox/left.jpg",
    "data/shaders/hello_cube_map/skybox/top.jpg",
    "data/shaders/hello_cube_map/skybox/bottom.jpg",
    "data/shaders/hello_cube_map/skybox/front.jpg",
    "data/shaders/hello_cube_map/skybox/back.jpg"
  };

  //unsigned int cubemapTexture = loadCubemap(faces);
};

void HelloTriangle::Begin() {

  camera_ = Camera();

  program_ = Shader("data/shaders/hello_triangle/triangle.vert", "data/shaders/hello_triangle/triangle.frag");
  cubemap_shader = Shader("data/shaders/hello_cube_map/skybox.vert", "data/shaders/hello_cube_map/skybox.frag");


  float cubeVertices[] = {
      // positions          // normals
      // Back face
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
      // Front face
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
      // Left face
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
      // Right face
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
      // Bottom face
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
      // Top face
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
  };

  float skyboxVertices[] = {
      // positions
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
      1.0f,  1.0f, -1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
      1.0f, -1.0f,  1.0f
  };

  //Load images
  glGenTextures(1, &cube_texture_);
  glBindTexture(GL_TEXTURE_2D, cube_texture_);

  // load and generate the texture
  int width, height, nrChannels;
  unsigned char *data = stbi_load("data/textures/container.jpg",
                                  &width,
                                  &height,
                                  &nrChannels,
                                  0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glGenTextures(1, &map_textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, map_textureID);

  int map_Width, map_Height, map_NrChannels;
  for(unsigned int i = 0; i < faces.size(); i++)
  {
    unsigned char *map_data = stbi_load(faces[i].c_str(), &map_Width, &map_Height, &map_NrChannels, 0);
    if (map_data){
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, map_Width, map_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, map_data);
      stbi_image_free(map_data);
    }
    else
    {
      std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(map_data);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // cube VAO
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

  // skybox VAO
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  program_.Use();
  program_.SetInt("texture1", 0);

  cubemap_shader.Use();
  cubemap_shader.SetInt("skybox", 0);
}

void HelloTriangle::End() {
  //Unload program/pipeline
  program_.Delete();
  cubemap_shader.Delete();

  glDeleteShader(vertexShader_);
  glDeleteShader(fragmentShader_);

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &skyboxVAO);
}

void HelloTriangle::Update(float dt) {
  UpdateCamera(dt);
  elapsed_time_ += dt;

  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 3D rotations
  program_.Use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  glm::mat4 projection = glm::mat4(1.0f);

  model = glm::rotate(model, elapsed_time_ * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
  projection = glm::perspective(glm::radians(camera_.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

  program_.SetMat4("model", model);
  program_.SetMat4("view", view);
  program_.SetMat4("projection", projection);

  //For Cubes
  glBindVertexArray(cubeVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, cube_texture_);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  //For Skybox
  glDepthFunc(GL_LEQUAL);
  cubemap_shader.Use();
  view = glm::mat4(glm::mat3(camera_.GetViewMatrix()));
  cubemap_shader.SetMat4("view", view);
  cubemap_shader.SetMat4("projection", projection);

  glBindVertexArray(skyboxVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, map_textureID);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}

void HelloTriangle::UpdateCamera(float dt) {

  // Get keyboard state
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  int mouseX;
  int mouseY;
  //Send WASD as enum
  // Camera controls
  if (state[SDL_SCANCODE_W]) {
    camera_.ProcessKeyboard(FORWARD, dt);
  }
  if (state[SDL_SCANCODE_S]) {
    camera_.ProcessKeyboard(BACKWARD, dt);
  }
  if (state[SDL_SCANCODE_A]) {
    camera_.ProcessKeyboard(LEFT, dt);
  }
  if (state[SDL_SCANCODE_D]) {
    camera_.ProcessKeyboard(RIGHT, dt);
  }

  Uint32 mouse_state = SDL_GetRelativeMouseState(&mouseX, &mouseY);
  if (mouse_state) {
    camera_.ProcesssMouseMovement(mouseX, mouseY, true);
  }
}

void HelloTriangle::OnEvent(const SDL_Event &event) {
  if (event.type == SDL_MOUSEWHEEL) {
    camera_.ProcessMouseScroll(event.wheel.y);
  }
}

}
int main(int argc, char **argv) {
  gpr5300::HelloTriangle scene;
  gpr5300::Engine engine(&scene);
  engine.Run();

  return EXIT_SUCCESS;
}

