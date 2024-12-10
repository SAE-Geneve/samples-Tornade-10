//
// Created by maxen on 02.12.2024.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef GPR5300_INCLUDE_CAMERA_H_
#define GPR5300_INCLUDE_CAMERA_H_

enum Camera_Movement{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class Camera
{
 public:
  // Camera Attributes
  glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);

  glm::vec3 Target = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 Direction = glm::normalize(Position - Target);

  glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 Right = glm::normalize(glm::cross(Up, Direction));

  glm::vec3 CameraUp = glm::cross(Direction, Right);

  glm::mat4 View = glm::lookAt(
      glm::vec3(0.0f, 0.0f, -3.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f));

  float Yaw = -90.0f;
  float Pitch = 0.0f;

  float MovementSpeed = 2.5f;
  float MouseSensitivity = 0.1f;
  float Zoom = 45.0f;

  Camera()
  {
    updateCameraVectors();
  }

  glm::mat4 GetViewMatrix()
  {
    return glm::lookAt(Position, Position + Front, Up);
  }

  void ProcessKeyboard(Camera_Movement direction, float deltaTime)
  {
    float velocity = MovementSpeed * deltaTime;

    switch (direction)
    {
      case FORWARD:
        Position += Front * velocity;
        break;
      case BACKWARD:
        Position -= Front * velocity;
        break;
      case LEFT:
        Position -= Right * velocity;
        break;
      case RIGHT:
        Position += Right * velocity;
        break;
      default:
        break;
    }

    updateCameraVectors();
  }

  void ProcesssMouseMovement(float xOffSet, float yOffSet, bool constrainPitch = true)
  {
    xOffSet *= MouseSensitivity;
    yOffSet *= MouseSensitivity;

    Yaw += xOffSet;
    Pitch += yOffSet;

    if (constrainPitch)
    {
      if (Pitch > 89.0f)
      {
        Pitch = 89.0f;
      }
      if (Pitch < -89.0f)
      {
        Pitch = -89.0f;
      }
    }

    updateCameraVectors();
  }

  void ProcessMouseScroll(float yOffSet)
  {
    Zoom -= yOffSet;
    if (Zoom < 1.0f)
      Zoom = 1.0f;
    if(Zoom > 45.0f)
      Zoom = 45.0f;
  }

 private:
  void updateCameraVectors()
  {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, CameraUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }
};

#endif //GPR5300_INCLUDE_CAMERA_H_
