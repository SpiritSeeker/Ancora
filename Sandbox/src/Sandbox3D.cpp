#include "Sandbox3D.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox3D::Sandbox3D()
  : Layer("Sandbox3D"), m_CameraController(glm::radians(45.0f), 1280.0f / 720.0f)
{
}

void Sandbox3D::OnAttach()
{
  m_Texture = Ancora::Texture2D::Create("Sandbox/assets/textures/pic.png");
}

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnUpdate(Ancora::Timestep ts)
{
  // Update
	m_FPS = 1 / ts;

	m_CameraController.OnUpdate(ts);

	// Render
  Ancora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Ancora::RenderCommand::Clear();

	Ancora::Renderer3D::BeginScene(m_CameraController.GetCamera());
  // Ancora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
  // Ancora::Renderer2D::DrawQuad({ 0.0f, -0.5f }, { 0.15f, 0.35f }, m_SquareColor);
  // Ancora::Renderer2D::DrawRotatedQuad({ 0.5f, 0.5f }, glm::radians(30.0f), { 0.5f, 0.5f }, m_Texture, 10, m_SquareColor);
  glm::vec3 center = glm::vec3(0.0f, 0.0f, -0.0f);
  float size = 1.0f;
  std::array<glm::vec3, 8> vertices;
  vertices[0] = { center.x - size / 2, center.y - size / 2, center.z + size / 2 };
  vertices[1] = { center.x + size / 2, center.y - size / 2, center.z + size / 2 };
  vertices[2] = { center.x + size / 2, center.y + size / 2, center.z + size / 2 };
  vertices[3] = { center.x - size / 2, center.y + size / 2, center.z + size / 2 };

  vertices[4] = { center.x + size / 2, center.y + size / 2, center.z - size / 2 };
  vertices[5] = { center.x - size / 2, center.y + size / 2, center.z - size / 2 };
  vertices[6] = { center.x - size / 2, center.y - size / 2, center.z - size / 2 };
  vertices[7] = { center.x + size / 2, center.y - size / 2, center.z - size / 2 };

  Ancora::Renderer3D::DrawCube(vertices, m_SquareColor);

  center = glm::vec3(5.0f, 5.0f, m_Back);
  std::array<glm::vec3, 4> vertices1;
  vertices1[0] = { center.x - size / 2, center.y - size / 2, center.z };
  vertices1[1] = { center.x + size / 2, center.y - size / 2, center.z };
  vertices1[2] = { center.x + size / 2, center.y + size / 2, center.z };
  vertices1[3] = { center.x - size / 2, center.y + size / 2, center.z };

  Ancora::Renderer3D::DrawQuad(vertices1, m_SquareColor);
  Ancora::Renderer3D::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
  ImGui::Begin("Test");
	ImGui::Text("FPS: %d", m_FPS);
	ImGui::End();

	ImGui::Begin("Settings");
	// ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
  ImGui::DragFloat("Back", &m_Back, 0.1f, -1000.0f, -4.0f);
	ImGui::End();
}

// void Sandbox3D::DrawCube(const glm::vec3& position, float size, const glm::vec4& color)
// {
//   glm::vec3 quad1 = position + glm::vec3({ -size / 2, -size / 2, size / 2 });
//   glm::vec3 quad2 = position + glm::vec3({ size / 2, 0, 0 });
//   glm::vec3 quad3 = position + glm::vec3({ 0, 0, -size / 2 });
//   glm::vec3 quad4 = position + glm::vec3({ -size / 2, 0, 0 });
//   glm::vec3 quad5 = position + glm::vec3({ 0, size / 2, 0 });
//   glm::vec3 quad6 = position + glm::vec3({ 0, -size / 2, 0 });
//
//   Ancora::Renderer3D::DrawQuad(quad1, color);
//   Ancora::Renderer3D::DrawQuad(quad%, color);
//   Ancora::Renderer3D::DrawQuad(quad%, color);
//   Ancora::Renderer3D::DrawQuad(quad%, color);
//   Ancora::Renderer3D::DrawQuad(quad%, color);
//   Ancora::Renderer3D::DrawQuad(quad%, color);
// }

void Sandbox3D::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
