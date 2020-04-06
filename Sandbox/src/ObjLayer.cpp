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
  m_VertexArray = Ancora::VertexArray::Create();

  std::vector<float> vertices;
  std::vector<float> uvs;
  std::vector<float> normals;

  LoadObj("Sandbox/assets/models/blender/cat/12221_Cat_v1_l3.obj", vertices, uvs, normals);

  m_VertexBuffer = Ancora::VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float));
  Ancora::BufferLayout layout = {
    { Ancora::ShaderDataType::Float3, "a_Position" }
  };
  m_VertexBuffer->SetLayout(layout);
  m_VertexArray->AddVertexBuffer(m_VertexBuffer);

  AE_TRACE("{0}", vertices.size() / 3);

  std::vector<uint32_t> indices;
  for (uint32_t i = 0; i < (int)(vertices.size() / 3); i++)
    indices.push_back(i);

  m_Num = indices.size();

  Ancora::Ref<Ancora::IndexBuffer> indexBuffer;
  indexBuffer = Ancora::IndexBuffer::Create(&indices[0], indices.size());
  m_VertexArray->SetIndexBuffer(indexBuffer);

  m_Shader = Ancora::Shader::Create("Sandbox/assets/shaders/FlatColor.glsl");
  m_Shader->Bind();
  m_CameraController.GetCamera().SetView({ 20.0f, 20.0f, 20.0f }, { 0.0f, 0.0f, 0.0f }, { -0.5f, 1.0f, -0.5f });
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
  m_Shader->Bind();
  m_Shader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
  m_Shader->SetMat4("u_Transform", glm::scale(glm::mat4(1.0f), glm::vec3({ 0.20f, 0.20f, 0.20f })));
  m_Shader->SetFloat4("u_Color", { 0.15f, 0.15f, 0.15f, 1.0f });

  glDrawElements(GL_TRIANGLES, m_Num, GL_UNSIGNED_INT, nullptr);
}

void ObjLayer::OnImGuiRender()
{
  // ImGui::Begin("Test");
	// ImGui::Text("FPS: %d", m_FPS);
	// ImGui::End();

  auto left = glm::cross(m_CameraController.GetCamera().GetCenter() - m_CameraController.GetCamera().GetPosition(), m_CameraController.GetCamera().GetUp());

	ImGui::Begin("Settings");
	// ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
  ImGui::Text("%f, %f, %f: Position", m_CameraController.GetCamera().GetCenter().x, m_CameraController.GetCamera().GetCenter().y, m_CameraController.GetCamera().GetCenter().z);
  ImGui::Text("%f, %f, %f: Left", left.x, left.y, left.z);
  ImGui::Text("%f, %f, %f: Up", m_CameraController.GetCamera().GetUp().x, m_CameraController.GetCamera().GetUp().y, m_CameraController.GetCamera().GetUp().z);
  ImGui::End();
}

void ObjLayer::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
