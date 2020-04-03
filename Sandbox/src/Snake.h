#pragma once

#include <vector>
#include <queue>
#include <glm/glm.hpp>

struct Block
{
  glm::vec2 Topleft;
  bool Movable = false;
  std::queue<glm::vec2> History;
};

enum Direction
{
  Up = 0, Down, Left, Right
};

class Snake
{
public:
  Snake(const glm::vec2& topleft = glm::vec2(0.0f), float size = 0.05f, uint32_t length = 1);
  ~Snake() = default;

  void MoveLeft();
  void MoveRight();
  void MoveUp();
  void MoveDown();

  bool InsideBounds(const glm::vec2& topleft = glm::vec2({ -1.0f, 1.0f }), const glm::vec2& bottomright = glm::vec2({ 1.0f, -1.0f }));
  bool CollisionWithSelf();
  bool CollisionWithFruit(const glm::vec2& fruit);

  bool AtTurnPoint();

  Direction GetDirection() { return m_Direction; }
  void SetDirection(Direction direction) { m_Direction = direction; }

  const float GetSpeed() const { return m_MoveSpeed; }
  const float GetBlockSize() const { return m_BlockSize; }
  const uint32_t GetLength() const { return m_Length; }

  void Grow();

  std::vector<Block> GetBlocks() { return m_Blocks; }
private:
  float m_BlockSize;
  uint32_t m_Length;
  Direction m_Direction;

  int m_FrameCount;

  float m_MoveSpeed = 0.5f;

  std::vector<Block> m_Blocks;
};
