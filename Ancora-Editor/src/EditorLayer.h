#pragma once

#include <Ancora.h>

namespace Ancora {

  class EditorLayer : public Layer
  {
  public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Event& e) override;
  private:
    OrthographicCameraController m_CameraController;

    Ref<VertexArray> m_VertexArray;
    Ref<Shader> m_Shader;

    Ref<Framebuffer> m_Framebuffer;

    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_SpriteSheet;
    Ref<SubTexture2D> m_BushTexture;

    int m_FPS;
    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

    uint32_t m_MapWidth, m_MapHeight;
    std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;
  };

}
