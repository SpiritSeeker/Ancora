#include <Ancora.h>

#include "Snake.h"

float eps = 0.005f;

Snake::Snake(const glm::vec2& topleft, float size, uint32_t length)
  : m_BlockSize(size), m_Length(length)
{
  m_Direction = Up;
  m_FrameCount = 0;
  Block head = { topleft, size };
  m_Blocks.push_back(head);
}

Snake::~Snake()
{

}


void Snake::MoveLeft()
{
  float distance = m_BlockSize / 5.0f;
  m_FrameCount += 1;
  // Major rework here
  for (auto& block : m_Blocks)
    block.Topleft.x -= distance;
}

void Snake::MoveRight()
{
  float distance = m_BlockSize / 5.0f;
  m_FrameCount += 1;
  // Major rework here
  for (auto& block : m_Blocks)
    block.Topleft.x += distance;
}

void Snake::MoveUp()
{
  float distance = m_BlockSize / 5.0f;
  m_FrameCount += 1;
  // Major rework here
  for (auto& block : m_Blocks)
    block.Topleft.y += distance;
}

void Snake::MoveDown()
{
  float distance = m_BlockSize / 5.0f;
  m_FrameCount += 1;
  // Major rework here
  for (auto& block : m_Blocks)
    block.Topleft.y -= distance;
}

bool Snake::InsideBounds(const glm::vec2& topleft, const glm::vec2& bottomright)
{
  return false;
}

bool Snake::CheckCollisionWithSelf()
{
  return false;
}

bool Snake::CheckCollisionWithFruit(const glm::vec2& topleft, float size)
{
  return false;
}

bool Snake::AtTurnPoint()
{
  if (m_FrameCount == 10)
  {
    m_FrameCount = 0;
    return true;
  }

  return false;
}

void Snake::Grow()
{

}
