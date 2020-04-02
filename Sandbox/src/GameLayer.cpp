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
  m_GrassTexture = Ancora::Texture2D::Create("Sandbox/assets/textures/white_grid.png");
  m_Snake = new Snake();
  m_SnakeDirection = m_Snake->GetDirection();
}

void GameLayer::OnDetach()
{
  delete m_Snake;
}

void GameLayer::OnUpdate(Ancora::Timestep ts)
{
  // Update
	m_FPS = 1 / ts;

  // Keyboard input

  if (Ancora::Input::IsKeyPressed(AE_KEY_UP))
    m_SnakeDirection = Up;
  else if (Ancora::Input::IsKeyPressed(AE_KEY_DOWN))
    m_SnakeDirection = Down;
  if (Ancora::Input::IsKeyPressed(AE_KEY_LEFT))
    m_SnakeDirection = Left;
  else if (Ancora::Input::IsKeyPressed(AE_KEY_RIGHT))
    m_SnakeDirection = Right;

  if (m_Snake->AtTurnPoint())
    m_Snake->SetDirection(m_SnakeDirection);

  // Snake movement control
  if (m_Snake->GetDirection() == Up) m_Snake->MoveUp();
  if (m_Snake->GetDirection() == Down) m_Snake->MoveDown();
  if (m_Snake->GetDirection() == Left) m_Snake->MoveLeft();
  if (m_Snake->GetDirection() == Right) m_Snake->MoveRight();

	m_CameraController.OnUpdate(ts);

	// Render
  Ancora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Ancora::RenderCommand::Clear();

	Ancora::Renderer2D::BeginScene(m_CameraController.GetCamera());
  Ancora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, m_GrassTexture);
  for (auto block : m_Snake->GetBlocks())
    Ancora::Renderer2D::DrawQuad({ block.Topleft.x + block.Size, block.Topleft.y - block.Size }, { block.Size, block.Size }, { 0.15f, 0.15f, 0.15f, 1.0f });
  Ancora::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
  ImGui::Begin("Test");
	ImGui::Text("FPS: %d", m_FPS);
	ImGui::End();
}

void GameLayer::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
