#pragma once

#include "PerspectiveCamera.h"

#include "Texture.h"
#include "Model3D.h"

namespace Ancora {

  #define AE_ENABLE_CUBEMAP       true
  #define AE_ENABLE_LIGHTING      true
  #define AE_SHOW_LIGHTSOURCE     true

  class Renderer3D
  {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const PerspectiveCamera& camera, bool enableCubeMap = false, bool enableLighting = false, bool showLightSource = false);
    static void EndScene();

    // Primitives
    static void DrawQuad(const std::array<glm::vec3, 4> &vertices, const glm::vec4& color);
    static void DrawCube(const std::array<glm::vec3, 8> &vertices, const glm::vec4& color, const glm::mat4& transform = glm::mat4(1.0f));
    static void DrawObject(const std::vector<VertexData3D>& vertexData, const glm::vec4& color, const glm::mat4& transform);
    static void DrawModel(Ref<Model3D> model, const glm::mat4& transform);
  };

}
