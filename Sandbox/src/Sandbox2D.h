#pragma once

#include <Ancora.h>

class Sandbox2D : public Ancora::Layer
{
public:
  Sandbox2D();
  virtual ~Sandbox2D() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Ancora::Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Ancora::Event& e) override;
private:
  Ancora::OrthographicCameraController m_CameraController;

  Ancora::Ref<Ancora::VertexArray> m_VertexArray;
  Ancora::Ref<Ancora::Shader> m_Shader;

  Ancora::Ref<Ancora::Framebuffer> m_Framebuffer;

  Ancora::Ref<Ancora::Texture2D> m_Texture;
  Ancora::Ref<Ancora::Texture2D> m_SpriteSheet;
  Ancora::Ref<Ancora::SubTexture2D> m_BushTexture;

  int m_FPS;
  glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

  uint32_t m_MapWidth, m_MapHeight;
  std::unordered_map<char, Ancora::Ref<Ancora::SubTexture2D>> m_TextureMap;
public:
  struct ProfileResult
  {
    const char* Name;
    float Time;
  };

  std::vector<ProfileResult> m_ProfileResults;
};
