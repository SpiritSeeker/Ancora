#include "Snake.h"

static bool PointInRect(const glm::vec2& point, const glm::vec2& topleft, float size)
{
  if (point.x < topleft.x)
    return false;
  if (point.x > topleft.x + size)
    return false;
  if (point.y > topleft.y)
    return false;
  if (point.y < topleft.y - size)
    return false;
  return true;
}

static bool RectInRect(const glm::vec2& topleft1, float size1, const glm::vec2& topleft2, float size2)
{
  if (PointInRect(topleft1, topleft2, size2))
    return true;
  if (PointInRect({ topleft1.x + size1, topleft1.y }, topleft2, size2))
    return true;
  if (PointInRect({ topleft1.x + size1, topleft1.y - size1 }, topleft2, size2))
    return true;
  if (PointInRect({ topleft1.x, topleft1.y - size1 }, topleft2, size2))
    return true;
  return false;
}

Snake::Snake(const glm::vec2& topleft, float size, uint32_t length)
  : m_BlockSize(size), m_Length(length)
{
  m_Direction = Up;
  m_FrameCount = 0;
  Block head;
  head.Topleft = topleft;
  for (uint32_t i  = 0; i < 10; i++)
    head.History.push(topleft);
  for (uint32_t i = 0; i < m_Length; i++)
    m_Blocks.push_back(head);
  m_Blocks[0].Movable = true;
}

void Snake::MoveLeft()
{
  float distance = m_BlockSize / 5.0f;
  m_FrameCount += 1;

  for (uint32_t i = m_Length - 1; i > 0; --i)
  {
    if (m_Blocks[i].Movable)
    {
      m_Blocks[i].Topleft = m_Blocks[i-1].History.front();
      m_Blocks[i].History.pop();
      m_Blocks[i].History.push(m_Blocks[i].Topleft);
    }
  }

  m_Blocks[0].Topleft.x -= distance;
  m_Blocks[0].History.pop();
  m_Blocks[0].History.push(m_Blocks[0].Topleft);
}

void Snake::MoveRight()
{
  float distance = m_BlockSize / 5.0f;
  m_FrameCount += 1;

  for (uint32_t i = m_Length - 1; i > 0; --i)
  {
    if (m_Blocks[i].Movable)
    {
      m_Blocks[i].Topleft = m_Blocks[i-1].History.front();
      m_Blocks[i].History.pop();
      m_Blocks[i].History.push(m_Blocks[i].Topleft);
    }
  }

  m_Blocks[0].Topleft.x += distance;
  m_Blocks[0].History.pop();
  m_Blocks[0].History.push(m_Blocks[0].Topleft);
}

void Snake::MoveUp()
{
  float distance = m_BlockSize / 5.0f;
  m_FrameCount += 1;

  for (uint32_t i = m_Length - 1; i > 0; --i)
  {
    if (m_Blocks[i].Movable)
    {
      m_Blocks[i].Topleft = m_Blocks[i-1].History.front();
      m_Blocks[i].History.pop();
      m_Blocks[i].History.push(m_Blocks[i].Topleft);
    }
  }

  m_Blocks[0].Topleft.y += distance;
  m_Blocks[0].History.pop();
  m_Blocks[0].History.push(m_Blocks[0].Topleft);
}

void Snake::MoveDown()
{
  float distance = m_BlockSize / 5.0f;
  m_FrameCount += 1;

  for (uint32_t i = m_Length - 1; i > 0; --i)
  {
    if (m_Blocks[i].Movable)
    {
      m_Blocks[i].Topleft = m_Blocks[i-1].History.front();
      m_Blocks[i].History.pop();
      m_Blocks[i].History.push(m_Blocks[i].Topleft);
    }
  }

  m_Blocks[0].Topleft.y -= distance;
  m_Blocks[0].History.pop();
  m_Blocks[0].History.push(m_Blocks[0].Topleft);
}

bool Snake::InsideBounds(const glm::vec2& topleft, const glm::vec2& bottomright)
{
  if (m_Blocks[0].Topleft.x < topleft.x)
    return false;
  if (m_Blocks[0].Topleft.y > topleft.y)
    return false;
  if (m_Blocks[0].Topleft.x + 2 * m_BlockSize > bottomright.x)
    return false;
  if (m_Blocks[0].Topleft.y - 2 * m_BlockSize < bottomright.y)
    return false;
  return true;
}

bool Snake::CollisionWithSelf()
{
  bool point1 = false;
  bool point2 = false;
  if (m_Direction == Up)
  {
    for (uint32_t i = 1; i < m_Length; i++)
    {
      if (PointInRect(m_Blocks[0].Topleft, m_Blocks[i].Topleft, 2 * m_BlockSize))
        point1 = true;
      if (PointInRect({ m_Blocks[0].Topleft.x + 2 * m_BlockSize, m_Blocks[0].Topleft.y }, m_Blocks[i].Topleft, 2 * m_BlockSize))
        point2 = true;
    }

    return point1 && point2;
  }

  if (m_Direction == Down)
  {
    for (uint32_t i = 1; i < m_Length; i++)
    {
      if (PointInRect({ m_Blocks[0].Topleft.x, m_Blocks[0].Topleft.y - 2 * m_BlockSize}, m_Blocks[i].Topleft, 2 * m_BlockSize))
        point1 = true;
      if (PointInRect({ m_Blocks[0].Topleft.x + 2 * m_BlockSize, m_Blocks[0].Topleft.y - 2 * m_BlockSize }, m_Blocks[i].Topleft, 2 * m_BlockSize))
        point2 = true;
    }

    return point1 && point2;
  }

  if (m_Direction == Left)
  {
    for (uint32_t i = 1; i < m_Length; i++)
    {
      if (PointInRect(m_Blocks[0].Topleft, m_Blocks[i].Topleft, 2 * m_BlockSize))
        point1 = true;
      if (PointInRect({ m_Blocks[0].Topleft.x, m_Blocks[0].Topleft.y - 2 * m_BlockSize }, m_Blocks[i].Topleft, 2 * m_BlockSize))
        point2 = true;
    }

    return point1 && point2;
  }

  if (m_Direction == Right)
  {
    for (uint32_t i = 1; i < m_Length; i++)
    {
      if (PointInRect({ m_Blocks[0].Topleft.x + 2 * m_BlockSize, m_Blocks[0].Topleft.y }, m_Blocks[i].Topleft, 2 * m_BlockSize))
        point1 = true;
      if (PointInRect({ m_Blocks[0].Topleft.x + 2 * m_BlockSize, m_Blocks[0].Topleft.y - 2 * m_BlockSize }, m_Blocks[i].Topleft, 2 * m_BlockSize))
        point2 = true;
    }

    return point1 && point2;
  }

  return false;
}

bool Snake::CollisionWithFruit(const glm::vec2& fruit)
{
  return PointInRect(fruit, m_Blocks[0].Topleft, 2 * m_BlockSize);
}

bool Snake::AtTurnPoint()
{
  if (m_FrameCount == 10)
  {
    m_FrameCount = 0;

    for (auto& block : m_Blocks)
      block.Movable = true;

    return true;
  }

  return false;
}

void Snake::Grow()
{
  Block newBlock = m_Blocks.back();
  newBlock.Movable = false;
  m_Blocks.push_back(newBlock);
  m_Length += 1;
}
