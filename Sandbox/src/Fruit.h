#pragma once

#include <glm/glm.hpp>

class Fruit
{
public:
  Fruit(const glm::vec2& initialSeed = glm::vec2({ -0.5f, -0.5f }), float size = 0.1f, int gridX = 20, int gridY = 20);
  ~Fruit() = default;

  void New();

  void Consumed() { m_Exists = false; }

  const bool Exists() const { return m_Exists; }
  const glm::vec2& GetLocation() const { return m_Location; }
  const float GetSize() const { return m_Size; }
private:
  glm::vec2 m_Location;
  float m_Size;
  int m_GridX;
  int m_GridY;
  bool m_Exists;
};
