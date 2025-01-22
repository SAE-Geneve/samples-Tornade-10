//
// Created by maxen on 22.01.2025.
//

#ifndef GPR5300_INCLUDE_VBO_H_
#define GPR5300_INCLUDE_VBO_H_

#include "shader.h"

class VBO {

 private:
  GLuint name_ = 0;

 public:

  void Creat(){
    glGenBuffers(1, &name_);
  }
  void Bind(){
    glBindBuffer(GL_ARRAY_BUFFER, name_);
  }
  void BindData(GLsizei size, const void *data, GLenum usage) const {
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
  }
  void Destroy(){
    glDeleteBuffers(1, &name_);
  }
};

#endif //GPR5300_INCLUDE_VBO_H_
