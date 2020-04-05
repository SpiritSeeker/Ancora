#include "aepch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ancora {

  PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : m_ProjectionMatrix(glm::perspective(fov, aspect, near, far))
  {
    m_ViewMatrix = glm::lookAt(m_Position, m_Center, m_Up);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

  void PerspectiveCamera::SetProjection(float fov, float aspect, float near, float far)
  {
    m_ProjectionMatrix = glm::perspective(fov, aspect, near, far);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

  void PerspectiveCamera::SetView(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
  {
    m_Position = position;
    m_Center = center;

    auto pos = m_Position - m_Center;

    if (glm::length(up) == 0)
    {
      double magXZ = glm::sqrt(glm::pow(pos.x, 2) + glm::pow(pos.z, 2));

      if (magXZ == 0)
        m_Up.y = 0;
      else
      {
        double mag = glm::length(pos);
        m_Up.x = (-1 * pos.y * pos.x) / (magXZ * mag);
        m_Up.y = magXZ / mag;
        m_Up.z = (-1 * pos.y * pos.z) / (magXZ * mag);
      }
    }
    else
      m_Up = up;

    m_ViewMatrix = glm::lookAt(m_Position, m_Center, m_Up);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

}
