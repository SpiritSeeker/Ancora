#pragma once

#include <Ancora.h>

#include <glad/glad.h>

class CubeMap : public Ancora::Layer
{
public:
  CubeMap();
  virtual ~CubeMap() = default;

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

  uint32_t m_TextureID;
};
