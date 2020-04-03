#pragma once

#include <Ancora.h>

#include "Snake.h"
#include "Fruit.h"

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
  Ancora::Ref<Ancora::Texture2D> m_WoodTexture;

  Snake* m_Snake;
  Direction m_SnakeDirection;

  Fruit* m_Fruit;

  bool m_GameRunning;
  float m_Scale = 0.95f;

  int m_FPS;
};
