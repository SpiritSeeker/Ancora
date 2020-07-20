#pragma once

#include "Ancora/Renderer/OrthographicCamera.h"
#include "Ancora/Core/Timestep.h"

#include "Ancora/Events/ApplicationEvent.h"
#include "Ancora/Events/MouseEvent.h"

namespace Ancora {

  struct Bounds
  {
    float Top;
    float Bottom;
    float Left;
    float Right;

    float GetWidth() { return Right - Left; }
    float GetHeight() { return Top - Bottom; }
  };

  class OrthographicCameraController
  {
  public:
    OrthographicCameraController(float aspectRatio, bool rotation = false);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    void OnResize(float width, float height);

    OrthographicCamera& GetCamera() { return m_Camera; }
    const OrthographicCamera& GetCamera() const { return m_Camera; }
    Bounds& GetBounds() { return m_Bounds; }

    void SetZoomLevel(float level);
  private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

    void CalculateView();
  private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    OrthographicCamera m_Camera;

    Bounds m_Bounds;

    bool m_Rotation;
    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraRotation = 0.0f;
    float m_CameraTranslationSpeed = 1.0f, m_CameraRotationSpeed = 1.0f;
  };

}
