#pragma once

#include <Ancora.h>

#include "ParticleSystem.h"

class ParticleLayer : public Ancora::Layer
{
public:
  ParticleLayer();
  virtual ~ParticleLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Ancora::Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Ancora::Event& e) override;
private:
  Ancora::OrthographicCameraController m_CameraController;
  int m_FPS;

  int m_Number = 20;
  float m_Gravity = 8.0f;

  ParticleProps m_Particle;
  ParticleSystem m_ParticleSystem;
};
