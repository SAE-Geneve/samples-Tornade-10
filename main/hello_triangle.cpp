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

  float elapsed_time_ = 0;

 private:
  Shader program_ = {}; // It's the OurShader
  Shader cubemap_shader = {};
  Shader framebuffer_shader = {};
  GLuint vertexShader_ = 0;
  GLuint fragmentShader_ = 0;
  GLuint cubemap_vao_ = 0;
  GLuint map_textureID = 0;

  unsigned int cube_texture_;

  Camera camera_;
  Framebuffer framebuffer_;

  //Refactor in a VAO/VBO Class DONE !
  VAO cubeVAO, skyboxVAO, framebufferVAO;
  VBO cubeVBO, skyboxVBO, framebufferVBO;

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
  framebuffer_shader = Shader("data/shaders/hello_triangle/framebuffer.vert", "data/shaders/hello_triangle/framebuffer.frag");


//  glGenFramebuffers(1, &FBO);
//  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
//
//  glGenTextures(1, &texture_color_buffer);
//  glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer, 0);
//
//  glGenRenderbuffers(1, &RBO);
//  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
//  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
//
//  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//    std::cout << "Framebuffer is not complete!" << std::endl;
//  }
//
//  glBindFramebuffer(GL_FRAMEBUFFER, 0);

 framebuffer_.FramebufferInit(800, 600);


  float cubeVertices[] = {
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

  float skyboxVertices[] = {
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

  float rectangleVertices[] = {
      // Positions    // Texture Coords
      -1.0f,  1.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 0.0f,

      -1.0f,  1.0f, 0.0f, 1.0f,
      1.0f, -1.0f, 1.0f, 0.0f,
      1.0f,  1.0f, 1.0f, 1.0f
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
    unsigned char *map_data = stbi_load(faces[i].c_str(), &map_Width, &map_Height, &map_NrChannels, 0);
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
  cubeVAO.Creat();
  cubeVBO.Creat();
  cubeVAO.Bind();
  cubeVBO.Bind();
  cubeVBO.BindData(sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

  // skybox VAO
  skyboxVAO.Creat();
  skyboxVBO.Creat();
  skyboxVAO.Bind();
  skyboxVBO.Bind();
  skyboxVBO.BindData(sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0); // Left astray
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);


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

  program_.Use();
  program_.SetInt("skybox", 0);

  cubemap_shader.Use();
  cubemap_shader.SetInt("skybox", 0);

  framebuffer_shader.Use();
  framebuffer_shader.SetInt("framebuffer", 0);
}

void HelloTriangle::End() {
  //Unload program/pipeline
  program_.Delete();
  cubemap_shader.Delete();
  framebuffer_shader.Delete();

  glDeleteShader(vertexShader_);
  glDeleteShader(fragmentShader_);

  framebuffer_.Delete();

  cubeVAO.Destroy();
  skyboxVAO.Destroy();
  framebufferVAO.Destroy();
}

void HelloTriangle::Update(float dt) {
  UpdateCamera(dt);
  elapsed_time_ += dt;

  glEnable(GL_DEPTH_TEST);

  //First pass
  framebuffer_.FirstPass();

  // 3D rotations
  program_.Use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  glm::mat4 projection = glm::mat4(1.0f);

  model = glm::rotate(model, elapsed_time_ * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
  projection = glm::perspective(glm::radians(camera_.Zoom), 1600.0f / 1200.0f, 0.1f, 100.0f);

  program_.SetMat4("model", model);
  program_.SetMat4("view", view);
  program_.SetMat4("projection", projection);
  program_.SetVec3("cameraPos", camera_.Position);

  //For Cubes
  cubeVAO.Bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, map_textureID);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  //For framebuffer

  //For Skybox
  glDepthFunc(GL_LEQUAL);
  cubemap_shader.Use();
  view = glm::mat4(glm::mat3(camera_.GetViewMatrix()));
  cubemap_shader.SetMat4("view", view);
  cubemap_shader.SetMat4("projection", projection);

  //Probably the full render, I sure hope so
  skyboxVAO.Bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, map_textureID);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);

  //Second pass
  framebuffer_.SecondPass(framebuffer_shader, framebufferVAO);
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

