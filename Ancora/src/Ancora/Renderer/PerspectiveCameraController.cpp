#include "aepch.h"
#include "PerspectiveCameraController.h"

#include "Ancora/Core/Input.h"
#include "Ancora/Core/KeyCodes.h"

namespace Ancora {

  PerspectiveCameraController::PerspectiveCameraController(float fov, float aspect)
    : m_FOV(fov), m_AspectRatio(aspect), m_Camera(m_FOV * m_ZoomLevel, m_AspectRatio, 0.1f, 100.0f)
  {
  }

  void PerspectiveCameraController::OnUpdate(Timestep ts)
  {
    m_CameraPosition = m_Camera.GetPosition();
    m_CameraCenter = m_Camera.GetCenter();
    m_CameraUp = m_Camera.GetUp();

    if (Input::IsKeyPressed(AE_KEY_W))
    {
      float upMag = glm::length(m_CameraUp);
			m_CameraPosition.x += m_CameraUp.x * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.y += m_CameraUp.y * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.z += m_CameraUp.z * ts * m_CameraTranslationSpeed / upMag;
    }

		else if (Input::IsKeyPressed(AE_KEY_S))
    {
      float upMag = glm::length(m_CameraUp);
    	m_CameraPosition.x -= m_CameraUp.x * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.y -= m_CameraUp.y * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.z -= m_CameraUp.z * ts * m_CameraTranslationSpeed / upMag;
    }

		if (Input::IsKeyPressed(AE_KEY_D))
    {
      glm::vec3 left = glm::cross(m_CameraPosition - m_CameraCenter, m_CameraUp);
      float leftMag = glm::length(left);
    	m_CameraPosition.x += left.x * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.y += left.y * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.z += left.z * ts * m_CameraTranslationSpeed / leftMag;
    }

    else if (Input::IsKeyPressed(AE_KEY_A))
    {
      auto left = glm::cross(m_CameraPosition - m_CameraCenter, m_CameraUp);
      float leftMag = glm::length(left);
			m_CameraPosition.x -= left.x * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.y -= left.y * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.z -= left.z * ts * m_CameraTranslationSpeed / leftMag;
    }

    if (Input::IsKeyPressed(AE_KEY_I))
    {
      float upMag = glm::length(m_CameraUp);
			m_CameraCenter.x += m_CameraUp.x * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.x += m_CameraUp.x * ts * m_CameraTranslationSpeed / upMag;
      m_CameraCenter.y += m_CameraUp.y * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.y += m_CameraUp.y * ts * m_CameraTranslationSpeed / upMag;
      m_CameraCenter.z += m_CameraUp.z * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.z += m_CameraUp.z * ts * m_CameraTranslationSpeed / upMag;
    }

		else if (Input::IsKeyPressed(AE_KEY_K))
    {
      float upMag = glm::length(m_CameraUp);
    	m_CameraCenter.x -= m_CameraUp.x * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.x -= m_CameraUp.x * ts * m_CameraTranslationSpeed / upMag;
      m_CameraCenter.y -= m_CameraUp.y * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.y -= m_CameraUp.y * ts * m_CameraTranslationSpeed / upMag;
      m_CameraCenter.z -= m_CameraUp.z * ts * m_CameraTranslationSpeed / upMag;
      m_CameraPosition.z -= m_CameraUp.z * ts * m_CameraTranslationSpeed / upMag;
    }

		if (Input::IsKeyPressed(AE_KEY_L))
    {
      glm::vec3 left = glm::cross(m_CameraPosition - m_CameraCenter, m_CameraUp);
      float leftMag = glm::length(left);
    	m_CameraCenter.x += left.x * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.x += left.x * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraCenter.y += left.y * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.y += left.y * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraCenter.z += left.z * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.z += left.z * ts * m_CameraTranslationSpeed / leftMag;
    }

    else if (Input::IsKeyPressed(AE_KEY_J))
    {
      auto left = glm::cross(m_CameraPosition - m_CameraCenter, m_CameraUp);
      float leftMag = glm::length(left);
			m_CameraCenter.x -= left.x * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.x -= left.x * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraCenter.y -= left.y * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.y -= left.y * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraCenter.z -= left.z * ts * m_CameraTranslationSpeed / leftMag;
      m_CameraPosition.z -= left.z * ts * m_CameraTranslationSpeed / leftMag;
    }

    m_Camera.SetView(m_CameraPosition, m_CameraCenter);

    m_CameraTranslationSpeed = m_ZoomLevel * glm::length(m_CameraCenter - m_CameraPosition);
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
    m_ZoomLevel = std::min(m_ZoomLevel, glm::radians(100.0f) / m_FOV);
    m_Camera.SetProjection(m_FOV * m_ZoomLevel, m_AspectRatio, 0.1f, 100.0f);
    return false;
  }

  bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
  {
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera.SetProjection(m_FOV * m_ZoomLevel, m_AspectRatio, 0.1f, 100.0f);
    return false;
  }

}
