#include "LightLayer.h"
#include "ObjParser.h"

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

  std::vector<float> vertex;
  std::vector<float> uvs;
  std::vector<float> normals;

  LoadObj("Sandbox/assets/models/Cerberus_by_Andrew_Maximov/Cerberus_LP.obj", vertex, uvs, normals);

  std::vector<float> v;
  m_Num = (int)(vertex.size() / 3);
  for (uint32_t i = 0; i < m_Num; i++)
  {
    v.push_back(vertex[3*i]);
    v.push_back(vertex[3*i+1]);
    v.push_back(vertex[3*i+2]);
    v.push_back(normals[3*i]);
    v.push_back(normals[3*i+1]);
    v.push_back(normals[3*i+2]);
    // v.push_back(0);
    // v.push_back(0);
    // v.push_back(0);
  }

  AE_TRACE("Vertices: {0}", m_Num);

  m_VertexBuffer = Ancora::VertexBuffer::Create(&v[0], v.size() * sizeof(float));
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

  std::vector<uint32_t> ind;
  for (uint32_t i = 0; i < m_Num; i++)
    ind.push_back(i);

  Ancora::Ref<Ancora::IndexBuffer> indexBuffer;
  Ancora::Ref<Ancora::IndexBuffer> lightIndexBuffer;

  indexBuffer = Ancora::IndexBuffer::Create(&ind[0], m_Num);
  m_VertexArray->SetIndexBuffer(indexBuffer);

  lightIndexBuffer = Ancora::IndexBuffer::Create(indices, 36);
  m_LightVertexArray->SetIndexBuffer(lightIndexBuffer);

  m_LightPosition = { -2.0f, 2.0f, 2.0f };

  m_Shader = Ancora::Shader::Create("Sandbox/assets/shaders/Lighting.glsl");
  m_Shader->Bind();
  m_Shader->SetMat4("u_Transform", glm::scale(glm::mat4(1.0f), glm::vec3({ 0.05f, 0.05f, 0.05f })));

  m_Shader->SetFloat3("u_Material.ambient", glm::vec3({ 0.7f, 0.7f, 0.7f }));
  m_Shader->SetFloat3("u_Material.diffuse", glm::vec3({ 0.7f, 0.7f, 0.7f }));
  m_Shader->SetFloat3("u_Material.specular", glm::vec3({ 0.5f, 0.5f, 0.5f }));
  m_Shader->SetFloat("u_Material.shininess", 32.0f);

  m_Shader->SetFloat3("u_Light.position", m_LightPosition);
  m_Shader->SetFloat3("u_Light.ambient", m_LightColor * glm::vec3(0.2f));
  m_Shader->SetFloat3("u_Light.diffuse", m_LightColor * glm::vec3(0.5f));
  m_Shader->SetFloat3("u_Light.specular", glm::vec3(1.0f));

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
  m_FPS = 1 / ts;

  Ancora::RenderCommand::SetClearColor({ 0.7f, 0.6f, 0.6f, 1.0f });
  Ancora::RenderCommand::Clear();

  // m_LightColor.x = 0.75f + 0.25f * sin(m_Time * 2.0f);
  // m_LightColor.y = 0.75f + 0.25f * sin(m_Time * 0.7f);
  // m_LightColor.z = 0.75f + 0.25f * sin(m_Time * 1.3f);

  // m_LightPosition.x = 3.0f * sin(m_Time * 0.8f);
  // m_LightPosition.z = 3.0f * cos(m_Time * 0.8f);

  m_CameraController.OnUpdate(ts);
  m_Shader->Bind();
  m_Shader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
  m_Shader->SetFloat3("u_CameraPosition", m_CameraController.GetCamera().GetPosition());

  m_Shader->SetFloat("u_Material.shininess", m_Metal);
  m_Shader->SetFloat3("u_Light.position", m_LightPosition);
  m_Shader->SetFloat3("u_Light.ambient", m_LightColor * glm::vec3(0.1f) + glm::vec3(0.1f));
  m_Shader->SetFloat3("u_Light.diffuse", m_LightColor * glm::vec3(0.5f));

  m_VertexArray->Bind();
  m_VertexArray->GetIndexBuffer()->Bind();
  glDrawElements(GL_TRIANGLES, m_Num, GL_UNSIGNED_INT, nullptr);

  // m_LightShader->Bind();
  // m_LightShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
  // m_LightShader->SetMat4("u_Transform", glm::translate(glm::mat4(1.0f), m_LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3({ 0.25f, 0.25f, 0.25f })));
  // m_LightShader->SetFloat3("u_Color", m_LightColor);
  // m_LightVertexArray->Bind();
  // m_LightVertexArray->GetIndexBuffer()->Bind();
  // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
}

void LightLayer::OnImGuiRender()
{
  ImGui::Begin("Test");
	ImGui::Text("FPS: %d", m_FPS);
	ImGui::End();

  auto left = glm::cross(m_CameraController.GetCamera().GetCenter() - m_CameraController.GetCamera().GetPosition(), m_CameraController.GetCamera().GetUp());

	ImGui::Begin("Settings");
	// ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
  // ImGui::Text("%f, %f, %f: Position", m_CameraController.GetCamera().GetCenter().x, m_CameraController.GetCamera().GetCenter().y, m_CameraController.GetCamera().GetCenter().z);
  ImGui::Text("%f, %f, %f: Left", m_LightColor.x, m_LightColor.y, m_LightColor.z);
  ImGui::DragFloat("Metalness", &m_Metal, 1, 1, 200);
  // ImGui::Text("%f, %f, %f: Up", m_CameraController.GetCamera().GetUp().x, m_CameraController.GetCamera().GetUp().y, m_CameraController.GetCamera().GetUp().z);
  ImGui::End();
}

void LightLayer::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
