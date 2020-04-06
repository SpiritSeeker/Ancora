#pragma once

#include <Ancora.h>

#include <glad/glad.h>

class LightLayer : public Ancora::Layer
{
public:
  LightLayer();
  virtual ~LightLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Ancora::Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Ancora::Event& e) override;
private:
  Ancora::PerspectiveCameraController m_CameraController;

  Ancora::Ref<Ancora::VertexArray> m_VertexArray;
  Ancora::Ref<Ancora::VertexBuffer> m_VertexBuffer;
  Ancora::Ref<Ancora::Shader> m_Shader;

  Ancora::Ref<Ancora::VertexArray> m_LightVertexArray;
  Ancora::Ref<Ancora::VertexBuffer> m_LightVertexBuffer;
  Ancora::Ref<Ancora::Shader> m_LightShader;

  glm::vec3 m_LightPosition;
  glm::vec3 m_LightColor = { 1.0f, 1.0f, 1.0f };
  float m_Time = 0.0f;
};
