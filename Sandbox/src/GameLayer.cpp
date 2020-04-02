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
  m_GrassTexture = Ancora::Texture2D::Create("Sandbox/assets/textures/grass.png");
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Ancora::Timestep ts)
{
  // Update
	m_FPS = 1 / ts;

	m_CameraController.OnUpdate(ts);

	// Render
  Ancora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Ancora::RenderCommand::Clear();

	Ancora::Renderer2D::BeginScene(m_CameraController.GetCamera());
  Ancora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, m_GrassTexture);
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
