#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace Ancora {

  struct VertexData3D
  {
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec3 Normal;
  };

  struct Mesh
  {
    std::vector<VertexData3D> Vertices;
    std::vector<uint32_t> Indices;
    std::vector<Ref<Texture2D>> DiffuseTextures;
    std::vector<Ref<Texture2D>> SpecularTextures;
    std::vector<Ref<Texture2D>> AmbientTextures;
    std::vector<Ref<Texture2D>> EmissiveTextures;
    std::vector<Ref<Texture2D>> HeightTextures;
    std::vector<Ref<Texture2D>> NormalsTextures;
    std::vector<Ref<Texture2D>> ShininessTextures;
    std::vector<Ref<Texture2D>> OpacityTextures;
    std::vector<Ref<Texture2D>> DisplacementTextures;
    std::vector<Ref<Texture2D>> LightmapTextures;
    std::vector<Ref<Texture2D>> ReflectionTextures;
    std::vector<Ref<Texture2D>> UnknownTextures;
  };

  class Model3D
  {
  public:
    Model3D() {}
    ~Model3D() = default;

    void SetName(const std::string& name) { m_Name = name; }

    void AddMesh(Mesh mesh) { m_Meshes.push_back(mesh); }

    const std::vector<Mesh> GetMesh() const { return m_Meshes; }
  private:
    std::string m_Name;
    std::vector<Mesh> m_Meshes;
  };

}
