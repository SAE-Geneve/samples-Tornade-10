//
// Created by maxen on 22.01.2025.
//

#ifndef GPR5300_INCLUDE_VAO_H_
#define GPR5300_INCLUDE_VAO_H_

#include "shader.h"
#include "GL/glew.h"

class VAO {

 private:
  GLuint name_ = 0;

 public:

  void Creat(){
    glGenVertexArrays(1, &name_);
  }
  void Bind(){
    glBindVertexArray(name_);
  }
  void Destroy(){
    glDeleteVertexArrays(1, &name_);
  }
};

#endif //GPR5300_INCLUDE_VAO_H_
