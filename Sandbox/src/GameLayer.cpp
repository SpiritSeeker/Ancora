#include "GameLayer.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GameLayer::GameLayer()
  : Layer("GameLayer"), m_CameraController(1280.0f / 720.0f)
{
}

void GameLayer::OnAttach()
{
  m_GrassTexture = Ancora::Texture2D::Create("Sandbox/assets/textures/grass_tile.png");
  m_WoodTexture = Ancora::Texture2D::Create("Sandbox/assets/textures/wood_tile.png");
  m_Snake = new Snake({ 0.0f, 0.0f }, 0.05f * m_Scale, 3);
  m_Fruit = new Fruit({ -0.5f * m_Scale, -0.5f * m_Scale }, 0.05f * m_Scale, 20, 20);
  m_SnakeDirection = m_Snake->GetDirection();
  m_GameRunning = true;
}

void GameLayer::OnDetach()
{
  delete m_Snake;
  delete m_Fruit;
}

void GameLayer::OnUpdate(Ancora::Timestep ts)
{
  // Update
	m_FPS = 1 / ts;

  if (m_GameRunning)
  {
    // Keyboard input
    if (Ancora::Input::IsKeyPressed(AE_KEY_UP))
      m_SnakeDirection = Up;
    if (Ancora::Input::IsKeyPressed(AE_KEY_DOWN))
      m_SnakeDirection = Down;
    if (Ancora::Input::IsKeyPressed(AE_KEY_LEFT))
      m_SnakeDirection = Left;
    if (Ancora::Input::IsKeyPressed(AE_KEY_RIGHT))
      m_SnakeDirection = Right;

    if (m_Snake->AtTurnPoint())
      m_Snake->SetDirection(m_SnakeDirection);

    // Snake movement control
    if (m_Snake->GetDirection() == Up) m_Snake->MoveUp();
    if (m_Snake->GetDirection() == Down) m_Snake->MoveDown();
    if (m_Snake->GetDirection() == Left) m_Snake->MoveLeft();
    if (m_Snake->GetDirection() == Right) m_Snake->MoveRight();

    // Game logic
    if (!m_Snake->InsideBounds() || m_Snake->CollisionWithSelf())
      m_GameRunning = false;

    if (!m_Fruit->Exists())
      m_Fruit->New();

    if (m_Snake->CollisionWithFruit(m_Fruit->GetLocation() + m_Fruit->GetSize()))
    {
      m_Fruit->Consumed();
      m_Snake->Grow();
    }

  }

  // Render
  Ancora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
  Ancora::RenderCommand::Clear();

  Ancora::Renderer2D::BeginScene(m_CameraController.GetCamera());
  Ancora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 1.0f, 1.0f }, m_WoodTexture);
  Ancora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { m_Scale, m_Scale }, m_GrassTexture);
  Ancora::Renderer2D::DrawQuad(glm::vec3(m_Fruit->GetLocation() + m_Fruit->GetSize(), -0.05f), { m_Fruit->GetSize(), m_Fruit->GetSize() }, { 0.8f, 0.2f, 0.3f, 1.0f });
  for (auto& block : m_Snake->GetBlocks())
    Ancora::Renderer2D::DrawQuad({ block.Topleft.x + m_Snake->GetBlockSize(), block.Topleft.y - m_Snake->GetBlockSize() }, { m_Snake->GetBlockSize(), m_Snake->GetBlockSize() }, { 0.15f, 0.15f, 0.15f, 1.0f });
  Ancora::Renderer2D::EndScene();

}

void GameLayer::OnImGuiRender()
{
  ImGui::Begin("Test");
	ImGui::Text("FPS: %d", m_FPS);
	ImGui::End();

  ImGui::Begin("Score");
  ImGui::Text("Snake Length: %d", m_Snake->GetLength());
  ImGui::End();
}

void GameLayer::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
