#pragma once

#include "Core.h"
#include "Ancora/Renderer/Model3D.h"

#include <assimp/scene.h>

namespace Ancora {

  class ModelLoader
  {
  public:
    static void Init();
    static void Shutdown();

    static Ref<Model3D> LoadModel(const std::string& filename);
  private:
    static void ProcessNode(aiNode* node, const aiScene* scene, Ref<Model3D> model, const std::string& currentDirectory);
    static Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& currentDirectory);
    static void LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::vector<Ref<Texture2D>>& textures, const std::string& currentDirectory, const Mesh& mesh);
  };

}
