#pragma once

#include <glm/glm.hpp>

namespace Ancora {

  class PerspectiveCamera
  {
  public:
    PerspectiveCamera(float fov, float aspect, float near, float far);

    void SetProjection(float fov, float aspect, float near, float far);

    const glm::vec3&  GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

    const glm::vec3& GetCenter() const { return m_Center; }
    void SetCenter(const glm::vec3& center) { m_Center = center; RecalculateViewMatrix(); }

    const glm::vec3& GetUp() const { return m_Up; }
    void SetUp(const glm::vec3& up) { m_Up = up; RecalculateViewMatrix(); }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
  private:
    void RecalculateViewMatrix();
  private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position = { 5.0f, 5.0f, 5.0f };
    glm::vec3 m_Center = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_Up = { -0.5f, 1.0f, -0.5f };
  };

}
