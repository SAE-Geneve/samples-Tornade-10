//
// Created by maxen on 04.02.2025.
//

#ifndef GPR5300_INCLUDE_MODEL_H_
#define GPR5300_INCLUDE_MODEL_H_

//TODO : install assimp on vcpkg
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"

class Model {
 public:
  Model(char *path){
    loadModel(path);
  }
  void Draw(Shader &shader){
    for (unsigned int i = 0; i < meshes.size(); i++){
      meshes[i].Draw(shader);
    }
  }

 private:
  std::vector<Mesh> meshes;
  std::string directory;

  void loadModel(std::string path){
    Assimp::Importer importer;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
      std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    }

    directory = path.substr(0, path.find_last_of("/"));

    processNode(scene->mRootNode, scene);
  }

  void processNode(aiNode *node, const aiScene *scene){
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
      processNode(node->mChildren[i], scene);
    }
  }

  //TODO : fix that pile of shit
  Mesh processMesh(aiMesh *mesh, const aiScene *scene){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
      Vertex vertex;
      // process vertex positions, normals and texture coordinates
      [...]
      vertices.push_back(vertex);
    }
    // process indices
    [...]
    // process material
    if(mesh->mMaterialIndex >= 0)
    {
      [...]
    }

    return Mesh(vertices, indices, textures);
  }

  //TODO : to do
  std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName){

  }

};

#endif //GPR5300_INCLUDE_MODEL_H_
