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

  void PerspectiveCamera::RecalculateViewMatrix()
  {
    m_ViewMatrix = glm::lookAt(m_Position, m_Center, m_Up);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

}
