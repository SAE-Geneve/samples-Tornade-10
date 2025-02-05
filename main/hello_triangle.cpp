#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

#include "engine.h"
#include "file_utility.h"
#include "scene.h"
#include "camera.h"
#include "shader.h"

#include "vao.h"
#include "vbo.h"
#include "framebuffer.h"

namespace gpr5300 {
class HelloTriangle final : public Scene {
 public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  void OnEvent(const SDL_Event &event) override;
  void UpdateCamera(const float dt) override;
  
 private:
  float elapsed_time_ = 0;
  
  Shader light_shader_ = {};
  Shader reflective_cube_shader_ = {};
  Shader cubemap_shader_ = {};
  Shader framebuffer_shader_ = {};
  Shader object_shader_ = {};
  
  GLuint map_textureID = 0;

  unsigned int cube_texture_;

  glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
  glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

  Camera camera_;
  Framebuffer framebuffer_;

  //Refactor in a VAO/VBO Class DONE !
  VAO cubeVAO, reflectiveCubeVAO, lightVAO, skyboxVAO, framebufferVAO;
  VBO cubeVBO, reflectiveCubeVBO, lightVBO, skyboxVBO, framebufferVBO;

  //  Cubemap faces
  static constexpr std::array<std::string_view , 6> faces
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

  light_shader_ = Shader("data/shaders/hello_triangle/cube.vert", "data/shaders/hello_triangle/cube.frag");
  object_shader_ = Shader("data/shaders/hello_triangle/materials.vert", "data/shaders/hello_triangle/materials.frag");
  reflective_cube_shader_ = Shader("data/shaders/hello_triangle/reflectivecube.vert", "data/shaders/hello_triangle/reflectivecube.frag");
  cubemap_shader_ = Shader("data/shaders/hello_cube_map/skybox.vert", "data/shaders/hello_cube_map/skybox.frag");
  framebuffer_shader_ = Shader("data/shaders/hello_triangle/framebuffer.vert", "data/shaders/hello_triangle/framebuffer.frag");

  framebuffer_.FramebufferInit(800, 600);

  static constexpr std::array cubeVertices{
      // positions          // normals
      //Back face
      0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

      //Front face
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

      //Left face
      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

      //Right face
      0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

      //Bottom face
      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

      //Top face
      0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
  };

  static constexpr std::array skyboxVertices{
      // positions
      -1.0f, 1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, 1.0f, -1.0f,
      -1.0f, 1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f, 1.0f, -1.0f,
      -1.0f, 1.0f, -1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,

      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f, -1.0f,
      1.0f, 1.0f, -1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f, 1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f, 1.0f,
      1.0f, -1.0f, 1.0f
  };

  static constexpr std::array rectangleVertices{
      // Positions    // Texture Coords
      -1.0f, 1.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 0.0f,

      -1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, -1.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 1.0f
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
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char *map_data = stbi_load(faces[i].data(), &map_Width, &map_Height, &map_NrChannels, 0);
    if (map_data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0,
                   GL_RGB,
                   map_Width,
                   map_Height,
                   0,
                   GL_RGB,
                   GL_UNSIGNED_BYTE,
                   map_data);
      stbi_image_free(map_data);
    } else {
      std::cerr << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
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
  cubeVAO.Creat();
  cubeVBO.Creat();
  cubeVAO.Bind();
  cubeVBO.Bind();
  cubeVBO.BindData(sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

  // light VAO
  lightVAO.Creat();
  lightVBO.Creat();
  lightVAO.Bind();
  lightVBO.Bind();
  lightVBO.BindData(sizeof(cubeVertices), cubeVertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);

  // reflective cube VAO
  reflectiveCubeVAO.Creat();
  reflectiveCubeVBO.Creat();
  reflectiveCubeVAO.Bind();
  reflectiveCubeVBO.Bind();
  reflectiveCubeVBO.BindData(sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

  // skybox VAO
  skyboxVAO.Creat();
  skyboxVBO.Creat();
  skyboxVAO.Bind();
  skyboxVBO.Bind();
  skyboxVBO.BindData(sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0); // Left astray
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);

  // framebuffer VAO
  framebufferVAO.Creat();
  framebufferVBO.Creat();
  framebufferVAO.Bind();
  framebufferVBO.Bind();
  framebufferVBO.BindData(sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

  light_shader_.Use();
  light_shader_.SetInt("light", 0);

  object_shader_.Use();
  object_shader_.SetInt("object", 0);

  reflective_cube_shader_.Use();
  reflective_cube_shader_.SetInt("skybox", 0);

  cubemap_shader_.Use();
  cubemap_shader_.SetInt("skybox", 0);

  framebuffer_shader_.Use();
  framebuffer_shader_.SetInt("framebuffer", 0);
}

void HelloTriangle::End() {
  //Unload program/pipeline
  reflective_cube_shader_.Delete();
  object_shader_.Delete();
  cubemap_shader_.Delete();
  framebuffer_shader_.Delete();

  framebuffer_.Delete();

  reflectiveCubeVAO.Destroy();
  lightVAO.Destroy();
  skyboxVAO.Destroy();
  framebufferVAO.Destroy();

  reflectiveCubeVBO.Destroy();
  lightVBO.Destroy();
  skyboxVBO.Destroy();
  framebufferVBO.Destroy();
}

void HelloTriangle::Update(float dt) {
  UpdateCamera(dt);
  elapsed_time_ += dt;

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  //First pass
  framebuffer_.FirstPass();

  //For reflective cubes
  reflective_cube_shader_.Use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(camera_.Zoom), 1600.0f / 1200.0f, 0.1f, 100.0f);

  model = glm::rotate(model, elapsed_time_ * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
  reflective_cube_shader_.SetMat4("model", model);
  reflective_cube_shader_.SetMat4("view", view);
  reflective_cube_shader_.SetMat4("projection", projection);
  reflective_cube_shader_.SetVec3("cameraPos", camera_.Position);

  reflectiveCubeVAO.Bind();
  glDrawArrays(GL_TRIANGLES, 0, 36);

  //For Lights
// Activate the light shader
  object_shader_.Use();

// Set up light properties
  glm::vec3 lightColor;
  lightColor.x = (std::sin(elapsed_time_ * 2.0f));
  lightColor.y = (std::sin(elapsed_time_ * 0.7f));
  lightColor.z = (std::sin(elapsed_time_ * 1.3f));
  glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
  glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

  object_shader_.SetVec3("light.position", lightPos);
  object_shader_.SetVec3("light.ambient", ambientColor);  // Dim ambient light
  object_shader_.SetVec3("light.diffuse", diffuseColor);  // Moderate diffuse light
  object_shader_.SetVec3("light.specular", glm::vec3(1.0f));   // Intense white specular light

// Set up material properties
  object_shader_.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
  object_shader_.SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
  object_shader_.SetVec3("material.specular", glm::vec3(0.5f)); // Subtle specular highlights
  object_shader_.SetFloat("material.shininess", 32.0f);

// Pass camera position for view-dependent effects
  object_shader_.SetVec3("viewPos", camera_.Position);

// Set transformation matrices
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 2.0f, 2.0f));                   // Position the light object
  model = glm::scale(model, glm::vec3(0.2f));          // Scale it down
  object_shader_.SetMat4("model", model);
  object_shader_.SetMat4("view", view);
  object_shader_.SetMat4("projection", projection);

// Render the light object
  cubeVAO.Bind();
  glDrawArrays(GL_TRIANGLES, 0, 36);

  //For light
  light_shader_.Use();

  model = glm::mat4(1.0f);
  model = glm::translate(model, lightPos);
  model = glm::scale(model, glm::vec3(0.2f));
  light_shader_.SetMat4("model", model);
  light_shader_.SetMat4("view", view);
  light_shader_.SetMat4("projection", projection);

  lightVAO.Bind();
  glDrawArrays(GL_TRIANGLES, 0, 36);

  //For Skybox
  glDepthFunc(GL_LEQUAL);
  cubemap_shader_.Use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, map_textureID);
  glBindVertexArray(0);

  view = glm::mat4(glm::mat3(camera_.GetViewMatrix()));
  cubemap_shader_.SetMat4("view", view);
  cubemap_shader_.SetMat4("projection", projection);

  skyboxVAO.Bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, map_textureID);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);

  //Second pass
  framebuffer_.SecondPass(framebuffer_shader_, framebufferVAO);
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