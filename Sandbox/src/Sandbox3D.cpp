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

  Ancora::Renderer3D::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
  ImGui::Begin("Test");
	ImGui::Text("FPS: %d", m_FPS);
	ImGui::End();

  auto left = glm::cross(m_CameraController.GetCamera().GetCenter() - m_CameraController.GetCamera().GetPosition(), m_CameraController.GetCamera().GetUp());

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
  ImGui::Text("%f, %f, %f: Position", m_CameraController.GetCamera().GetPosition().x, m_CameraController.GetCamera().GetPosition().y, m_CameraController.GetCamera().GetPosition().z);
  ImGui::Text("%f, %f, %f: Left", left.x, left.y, left.z);
  ImGui::Text("%f, %f, %f: Up", m_CameraController.GetCamera().GetUp().x, m_CameraController.GetCamera().GetUp().y, m_CameraController.GetCamera().GetUp().z);
  ImGui::End();
}

void Sandbox3D::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
