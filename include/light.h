//
// Created by maxen on 04.12.2024.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef GPR5300_INCLUDE_LIGHT_H_
#define GPR5300_INCLUDE_LIGHT_H_

class Light {
 public:
  glm::vec3 position;
  glm::vec3 color;

  Light(const glm::vec3& position_, const glm::vec3& color_) : position(position_), color(color_) {}



};

#endif //GPR5300_INCLUDE_LIGHT_H_
