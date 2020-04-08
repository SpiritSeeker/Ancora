#pragma once

#include <Ancora.h>

#include <glad/glad.h>

class ObjLayer : public Ancora::Layer
{
public:
  ObjLayer();
  virtual ~ObjLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Ancora::Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Ancora::Event& e) override;
private:
  Ancora::PerspectiveCameraController m_CameraController;

  Ancora::Ref<Ancora::Model3D> m_Model;
};
