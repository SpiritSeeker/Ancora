#pragma once

#include <Ancora.h>

#include "Snake.h"

class GameLayer : public Ancora::Layer
{
public:
  GameLayer();
  virtual ~GameLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Ancora::Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Ancora::Event& e) override;
private:
  Ancora::OrthographicCameraController m_CameraController;

  Ancora::Ref<Ancora::VertexArray> m_VertexArray;
  Ancora::Ref<Ancora::Shader> m_Shader;

  Ancora::Ref<Ancora::Texture2D> m_GrassTexture;

  Snake* m_Snake;
  Direction m_SnakeDirection;

  int m_FPS;
};
