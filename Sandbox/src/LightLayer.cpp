#include "LightLayer.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

LightLayer::LightLayer()
  : Layer("LightLayer"), m_CameraController(glm::radians(45.0f), 16.0 / 9.0)
{
}

void LightLayer::OnAttach()
{
  m_VertexArray = Ancora::VertexArray::Create();
  m_LightVertexArray = Ancora::VertexArray::Create();

  float vertices[] = {
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f
  };

  m_VertexBuffer = Ancora::VertexBuffer::Create(vertices, 6 * 4 * 6 * sizeof(float));
  m_LightVertexBuffer = Ancora::VertexBuffer::Create(vertices, 6 * 4 * 6 * sizeof(float));
  Ancora::BufferLayout layout = {
    { Ancora::ShaderDataType::Float3, "a_Position" },
    { Ancora::ShaderDataType::Float3, "a_Normal" }
  };
  m_VertexBuffer->SetLayout(layout);
  m_VertexArray->AddVertexBuffer(m_VertexBuffer);
  m_LightVertexBuffer->SetLayout(layout);
  m_LightVertexArray->AddVertexBuffer(m_LightVertexBuffer);

  uint32_t* indices = new uint32_t[36];

  uint32_t offset = 0;
  for (uint32_t i = 0; i < 36; i += 6)
  {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;

    indices[i + 3] = offset + 2;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 0;

    offset += 4;
  }

  Ancora::Ref<Ancora::IndexBuffer> indexBuffer;
  indexBuffer = Ancora::IndexBuffer::Create(indices, 36);
  m_VertexArray->SetIndexBuffer(indexBuffer);
  m_LightVertexArray->SetIndexBuffer(indexBuffer);

  glm::vec3 lightPosition = { -2.0f, 2.0f, 2.0f };

  m_Shader = Ancora::Shader::Create("Sandbox/assets/shaders/Lighting.glsl");
  m_Shader->Bind();
  m_Shader->SetMat4("u_Transform", glm::mat4(1.0f));
  m_Shader->SetFloat3("u_Color", glm::vec3({ 1.0f, 0.5f, 0.3f }));
  m_Shader->SetFloat3("u_LightColor", glm::vec3(1.0f));
  m_Shader->SetFloat3("u_LightPosition", lightPosition);

  m_LightShader = Ancora::Shader::Create("Sandbox/assets/shaders/LightCube.glsl");
  m_LightShader->Bind();
  m_LightShader->SetMat4("u_Transform", glm::translate(glm::mat4(1.0f), lightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3({ 0.25f, 0.25f, 0.25f })));

  m_CameraController.GetCamera().SetView({ 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { -0.5f, 1.0f, -0.5f });
}

void LightLayer::OnDetach()
{
}

void LightLayer::OnUpdate(Ancora::Timestep ts)
{
  Ancora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
  Ancora::RenderCommand::Clear();

  m_CameraController.OnUpdate(ts);
  m_Shader->Bind();
  m_Shader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
  m_Shader->SetFloat3("u_CameraPosition", m_CameraController.GetCamera().GetPosition());
  m_VertexArray->Bind();
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

  m_LightShader->Bind();
  m_LightShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
  m_LightVertexArray->Bind();
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
}

void LightLayer::OnImGuiRender()
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

void LightLayer::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
