#include "ObjLayer.h"
#include "ObjParser.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ObjLayer::ObjLayer()
  : Layer("ObjLayer"), m_CameraController(glm::radians(45.0f), 1280.0f / 720.0f)
{
}

void ObjLayer::OnAttach()
{
  std::vector<float> vertices;
  std::vector<float> uvs;
  std::vector<float> normals;

  LoadObj("Sandbox/assets/models/Cerberus_by_Andrew_Maximov/Cerberus_LP.obj", vertices, uvs, normals);

  m_Num = vertices.size() / 3;
  AE_TRACE("{0}", m_Num);

  for (uint32_t i = 0; i < m_Num; i++)
  {
    Ancora::VertexData3D vertex;
    vertex.Position = { vertices[3*i], vertices[3*i+1], vertices[3*i+2] };
    vertex.TexCoord = { 0.0f, 0.0f };
    vertex.Normal = { normals[3*i], normals[3*i+1], normals[3*i+2] };
    vertexData.push_back(vertex);
  }

  m_CameraController.GetCamera().SetView({ 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { -0.5f, 1.0f, -0.5f });
  AE_TRACE("Loaded!");
}

void ObjLayer::OnDetach()
{
}

void ObjLayer::OnUpdate(Ancora::Timestep ts)
{
  Ancora::RenderCommand::SetClearColor({ 0.7f, 0.6f, 0.6f, 1.0f });
  Ancora::RenderCommand::Clear();

  m_CameraController.OnUpdate(ts);

  Ancora::Renderer3D::BeginScene(m_CameraController.GetCamera(), false, true);
  Ancora::Renderer3D::DrawObject(vertexData, { 0.4f, 0.4f, 0.4f, 1.0f }, glm::scale(glm::mat4(1.0f), glm::vec3({ 0.05f, 0.05f, 0.05f })));
  Ancora::Renderer3D::EndScene();

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

  Ancora::Renderer3D::DrawCube(vertices, glm::vec4(1.0f), glm::translate(glm::mat4(1.0f), glm::vec3({ -2.0f, 2.0f, 2.0f })) * glm::scale(glm::mat4(1.0f), glm::vec3(0.25f)));
  Ancora::Renderer3D::EndScene();
}

void ObjLayer::OnImGuiRender()
{
  auto left = glm::cross(m_CameraController.GetCamera().GetCenter() - m_CameraController.GetCamera().GetPosition(), m_CameraController.GetCamera().GetUp());

	ImGui::Begin("Settings");
  ImGui::Text("%f, %f, %f: Position", m_CameraController.GetCamera().GetCenter().x, m_CameraController.GetCamera().GetCenter().y, m_CameraController.GetCamera().GetCenter().z);
  ImGui::Text("%f, %f, %f: Left", left.x, left.y, left.z);
  ImGui::Text("%f, %f, %f: Up", m_CameraController.GetCamera().GetUp().x, m_CameraController.GetCamera().GetUp().y, m_CameraController.GetCamera().GetUp().z);
  ImGui::End();
}

void ObjLayer::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
