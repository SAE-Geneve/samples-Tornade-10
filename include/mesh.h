//
// Created by maxen on 03.02.2025.
//

#ifndef GPR5300_INCLUDE_MESH_H_
#define GPR5300_INCLUDE_MESH_H_

#include <vector>
#include <string>

#include "shader.h"
#include "vao.h"
#include "vbo.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
  int m_BoneIDs[MAX_BONE_INFLUENCE];
  float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh{
 public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<Vertex> vertices_, std::vector<unsigned int> indices_, std::vector<Texture> textures_){
    this->vertices = vertices_;
    this->indices = indices_;
    this->textures = textures_;

    setupMesh();
  }

  void Draw(Shader &shader){
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for(unsigned int i = 0; i < textures.size(); i++){
      glActiveTexture(GL_TEXTURE0 + i);

      std::string number;
      std::string name = textures[i].type;

      if (name == "texture_diffuse"){
        number = std::to_string(diffuseNr++);
      }
      else if (name == "texture_specular"){
        number = std::to_string(specularNr++);
      }
      else if(name == "texture_normal") {
        number = std::to_string(normalNr++);
      }// transfer unsigned int to string
      else if(name == "texture_height") {
        number = std::to_string(heightNr++); // transfer unsigned int to string
      }

      shader.SetInt(("material." + name + number).c_str(), i);
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    vao_.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
  }

 private:
  VAO vao_;
  VBO vbo_;
  unsigned int ebo_;

  void setupMesh(){
    vao_.Creat();
    vbo_.Creat();
    glGenBuffers(1, &ebo_);

    vao_.Bind();
    vbo_.Bind();

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

    glBindVertexArray(0);
  }
};

#endif //GPR5300_INCLUDE_MESH_H_
