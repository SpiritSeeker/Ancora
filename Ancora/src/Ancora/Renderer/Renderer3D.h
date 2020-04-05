#pragma once

#include "PerspectiveCamera.h"

#include "Texture.h"

namespace Ancora {

  class Renderer3D
  {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const PerspectiveCamera& camera);
    static void EndScene();

    // Primitives
    static void DrawQuad(const std::array<glm::vec3, 4> &vertices, const glm::vec4& color);
    static void DrawCube(const std::array<glm::vec3, 8> &vertices, const glm::vec4& color, const glm::mat4& transform = glm::mat4(1.0f));
  };

}
