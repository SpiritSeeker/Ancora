#pragma once

#include <Ancora.h>

class Sandbox3D : public Ancora::Layer
{
public:
  Sandbox3D();
  virtual ~Sandbox3D() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Ancora::Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Ancora::Event& e) override;
// private:
  // void DrawCube(const glm::vec3& position, float size, const glm::vec4& color = glm::vec4(1.0f));
private:
  Ancora::PerspectiveCameraController m_CameraController;

  Ancora::Ref<Ancora::VertexArray> m_VertexArray;
  Ancora::Ref<Ancora::Shader> m_Shader;

  Ancora::Ref<Ancora::Texture2D> m_Texture;

  float m_Back = -10.0f;

  int m_FPS;
  glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
