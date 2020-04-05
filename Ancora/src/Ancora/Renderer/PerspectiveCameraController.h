#pragma once

#include "Ancora/Renderer/PerspectiveCamera.h"
#include "Ancora/Core/Timestep.h"

#include "Ancora/Events/ApplicationEvent.h"
#include "Ancora/Events/MouseEvent.h"

namespace Ancora {

  class PerspectiveCameraController
  {
  public:
    PerspectiveCameraController(float fov, float aspect);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    PerspectiveCamera& GetCamera() { return m_Camera; }
    const PerspectiveCamera& GetCamera() const { return m_Camera; }
  private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);
  private:
    float m_FOV;
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    PerspectiveCamera m_Camera;

    glm::vec3 m_CameraPosition = { 5.0f, 5.0f, 5.0f };
    glm::vec3 m_CameraCenter = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_CameraUp = { -0.5f, 1.0f, -0.5f };

    float m_CameraTranslationSpeed = 10.0f;
  };

}
