#pragma once

#include <glm/glm.hpp>

namespace Ancora {

  class PerspectiveCamera
  {
  public:
    PerspectiveCamera(float fov, float aspect, float near, float far);

    void SetProjection(float fov, float aspect, float near, float far);

    void SetView(const glm::vec3& position, const glm::vec3& center = glm::vec3(0.0f), const glm::vec3& up = glm::vec3(0.0f));

    const glm::vec3& GetPosition() const { return m_Position; }
    const glm::vec3& GetCenter() const { return m_Center; }
    const glm::vec3& GetUp() const { return m_Up; }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
  private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position = { 5.0f, 5.0f, 5.0f };
    glm::vec3 m_Center = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_Up = { -0.5f, 1.0f, -0.5f };
  };

}
