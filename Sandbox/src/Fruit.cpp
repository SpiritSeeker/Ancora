#include "Fruit.h"

#include <cstdlib>

Fruit::Fruit(const glm::vec2& initialSeed, float size, int gridX, int gridY)
  : m_Location(initialSeed), m_Size(size), m_GridX(gridX), m_GridY(gridY)
{
  m_Exists = true;
}

void Fruit::New()
{
  m_Location.x = (float)((rand() % m_GridX) - (m_GridX / 2)) * 2 * m_Size;
  m_Location.y = (float)((rand() % m_GridY) - (m_GridY / 2)) * 2 * m_Size;
  m_Exists = true;
}
