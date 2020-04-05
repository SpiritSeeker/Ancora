#include "aepch.h"
#include "PerspectiveCameraController.h"

namespace Ancora {

  PerspectiveCameraController::PerspectiveCameraController(float fov, float aspect)
    : m_FOV(fov), m_AspectRatio(aspect), m_Camera(m_FOV * m_ZoomLevel, m_AspectRatio, 1.0f, 50.0f)
  {
  }

  void PerspectiveCameraController::OnUpdate(Timestep ts)
  {
  }

  void PerspectiveCameraController::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(AE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(AE_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
  }

  bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
  {
    m_ZoomLevel -= e.GetYOffset() * 0.15f;
    m_ZoomLevel = std::max(m_ZoomLevel, glm::radians(1.0f) / m_FOV);
    m_ZoomLevel = std::min(m_ZoomLevel, glm::radians(150.0f) / m_FOV);
    m_Camera.SetProjection(m_FOV * m_ZoomLevel, m_AspectRatio, 1.0f, 50.0f);
    return false;
  }

  bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
  {
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera.SetProjection(m_FOV * m_ZoomLevel, m_AspectRatio, 1.0f, 50.0f);
    return false;
  }

}
