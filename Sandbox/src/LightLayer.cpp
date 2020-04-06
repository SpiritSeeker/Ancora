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
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,

     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f
  };

  m_VertexBuffer = Ancora::VertexBuffer::Create(vertices, 6 * 4 * 8 * sizeof(float));
  m_LightVertexBuffer = Ancora::VertexBuffer::Create(vertices, 6 * 4 * 8 * sizeof(float));
  Ancora::BufferLayout layout = {
    { Ancora::ShaderDataType::Float3, "a_Position" },
    { Ancora::ShaderDataType::Float3, "a_Normal" },
    { Ancora::ShaderDataType::Float2, "a_TexCoords" }
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

  m_LightPosition = { -2.0f, 2.0f, 2.0f };

  m_Shader = Ancora::Shader::Create("Sandbox/assets/shaders/Lighting.glsl");
  m_Shader->Bind();
  m_Shader->SetMat4("u_Transform", glm::mat4(1.0f));

  m_Shader->SetInt("u_Material.diffuse", 0);
  m_Shader->SetInt("u_Material.specular", 1);
  m_Shader->SetFloat("u_Material.shininess", 32.0f);

  m_Shader->SetFloat3("u_Light.position", m_LightPosition);
  m_Shader->SetFloat3("u_Light.ambient", m_LightColor * glm::vec3(0.2f));
  m_Shader->SetFloat3("u_Light.diffuse", m_LightColor * glm::vec3(0.5f));
  m_Shader->SetFloat3("u_Light.specular", glm::vec3(1.0f));

  m_DiffuseTexture = Ancora::Texture2D::Create("Sandbox/assets/textures/container2.png");
  m_DiffuseTexture->Bind(0);

  m_SpecularTexture = Ancora::Texture2D::Create("Sandbox/assets/textures/container2_specular.png");
  m_SpecularTexture->Bind(1);

  m_LightShader = Ancora::Shader::Create("Sandbox/assets/shaders/LightCube.glsl");
  m_LightShader->Bind();
  m_LightShader->SetMat4("u_Transform", glm::translate(glm::mat4(1.0f), m_LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3({ 0.25f, 0.25f, 0.25f })));
  m_LightShader->SetFloat3("u_Color", m_LightColor);

  m_CameraController.GetCamera().SetView({ 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { -0.5f, 1.0f, -0.5f });
}

void LightLayer::OnDetach()
{
}

void LightLayer::OnUpdate(Ancora::Timestep ts)
{
  m_Time += ts;

  Ancora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
  Ancora::RenderCommand::Clear();

  // m_LightColor.x = sin(m_Time * 2.0f);
  // m_LightColor.y = sin(m_Time * 0.7f);
  // m_LightColor.z = sin(m_Time * 1.3f);

  // m_LightPosition.x = 3.0f * sin(m_Time * 0.8f);
  // m_LightPosition.z = 3.0f * cos(m_Time * 0.8f);

  m_CameraController.OnUpdate(ts);
  m_Shader->Bind();
  m_Shader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
  m_Shader->SetFloat3("u_CameraPosition", m_CameraController.GetCamera().GetPosition());

  m_Shader->SetFloat3("u_Light.position", m_LightPosition);
  m_Shader->SetFloat3("u_Light.ambient", m_LightColor * glm::vec3(0.1f) + glm::vec3(0.1f));
  m_Shader->SetFloat3("u_Light.diffuse", m_LightColor * glm::vec3(0.5f));

  m_VertexArray->Bind();
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

  m_LightShader->Bind();
  m_LightShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
  m_LightShader->SetMat4("u_Transform", glm::translate(glm::mat4(1.0f), m_LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3({ 0.25f, 0.25f, 0.25f })));
  m_LightShader->SetFloat3("u_Color", m_LightColor);
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
  // ImGui::Text("%f, %f, %f: Position", m_CameraController.GetCamera().GetCenter().x, m_CameraController.GetCamera().GetCenter().y, m_CameraController.GetCamera().GetCenter().z);
  ImGui::Text("%f, %f, %f: Left", m_LightColor.x, m_LightColor.y, m_LightColor.z);
  // ImGui::Text("%f, %f, %f: Up", m_CameraController.GetCamera().GetUp().x, m_CameraController.GetCamera().GetUp().y, m_CameraController.GetCamera().GetUp().z);
  ImGui::End();
}

void LightLayer::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
