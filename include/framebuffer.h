//
// Created by maxen on 22.01.2025.
//

#ifndef GPR5300_INCLUDE_FRAMEBUFFER_H_
#define GPR5300_INCLUDE_FRAMEBUFFER_H_

#include "shader.h"
#include "vao.h"

class Framebuffer{
 private:
  unsigned int FBO_;
  unsigned int RBO_;
  unsigned int texture_color_buffer_;

 public:

  Framebuffer() : FBO_(0), RBO_(0), texture_color_buffer_(0) {};

  void FramebufferInit(int width, int height){
    glGenFramebuffers(1, &FBO_);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);

    glGenTextures(1, &texture_color_buffer_);
    glBindTexture(GL_TEXTURE_2D, texture_color_buffer_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer_, 0);

    glGenRenderbuffers(1, &RBO_);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cout << "Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  };

  void FirstPass(){
    // std::cout << "First Pass" << "\n";

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
  };

  void SecondPass(Shader& framebuffer_shader_, VAO vao){
    // std::cout << "Second Pass" << "\n";

    glDisable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    framebuffer_shader_.Use();

    vao.Bind();
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture_color_buffer_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  };

  void Delete() {
    glDeleteFramebuffers(1, &FBO_);
    glDeleteRenderbuffers(1, &RBO_);
    glDeleteTextures(1, &texture_color_buffer_);
  }
};

#endif //GPR5300_INCLUDE_FRAMEBUFFER_H_
