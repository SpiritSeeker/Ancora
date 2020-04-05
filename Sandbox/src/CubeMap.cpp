#include "CubeMap.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

CubeMap::CubeMap()
  : Layer("CubeMap"), m_CameraController(glm::radians(45.0f), 1280.0f / 720.0f)
{
}

void CubeMap::OnAttach()
{
  // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  m_VertexArray = Ancora::VertexArray::Create();

  float vertices[] = {
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
  };

  m_VertexBuffer = Ancora::VertexBuffer::Create(vertices, 6 * 4 * 3 * sizeof(float));
  Ancora::BufferLayout layout = {
    { Ancora::ShaderDataType::Float3, "a_Position" }
  };
  m_VertexBuffer->SetLayout(layout);
  m_VertexArray->AddVertexBuffer(m_VertexBuffer);

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

  glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_TextureID);

  AE_INFO("{0}", m_TextureID);

  std::array<std::string, 6> textureFaces = {
    "Sandbox/assets/cubemaps/tears_of_steel/px.png",
    "Sandbox/assets/cubemaps/tears_of_steel/nx.png",
    "Sandbox/assets/cubemaps/tears_of_steel/py.png",
    "Sandbox/assets/cubemaps/tears_of_steel/ny.png",
    "Sandbox/assets/cubemaps/tears_of_steel/pz.png",
    "Sandbox/assets/cubemaps/tears_of_steel/nz.png"
  };

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  int width, height, nrChannels;

  for (GLuint i = 0; i < 6; i++)
  {
    unsigned char* data = stbi_load(textureFaces[i].c_str(), &width, &height, &nrChannels, 0);

    GLenum internalFormat = 0, dataFormat = 0;
    if (nrChannels == 4)
    {
      internalFormat = GL_RGBA;
      dataFormat = GL_RGBA;
    }
    else if (nrChannels == 3)
    {
      internalFormat = GL_RGB8;
      dataFormat = GL_RGB;
    }

    AE_TRACE("{0}", nrChannels);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }

  m_Shader = Ancora::Shader::Create("Sandbox/assets/shaders/CubeMap.glsl");
  m_Shader->Bind();
  m_Shader->SetInt("u_Skybox", 1);
  m_CameraController.GetCamera().SetView({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -0.5f }, { 0.0f, 1.0f, 0.0f });

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::OnDetach()
{
}

void CubeMap::OnUpdate(Ancora::Timestep ts)
{
  Ancora::RenderCommand::SetClearColor({ 0.7f, 0.1f, 0.1f, 1.0f });
  Ancora::RenderCommand::Clear();

  glDepthMask(GL_FALSE);

  m_CameraController.OnUpdate(ts);
  m_Shader->Bind();
  m_Shader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
  m_Shader->SetFloat4("u_Color", { 0.5f, 0.2f, 0.7f, 1.0f });

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
  glBindTextureUnit(1, m_TextureID);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
  glDepthMask(GL_TRUE);
  m_Shader->Unbind();

  // Ancora::Renderer3D::BeginScene(m_CameraController.GetCamera());
  //
  // glm::vec3 center = glm::vec3(0.0f, 0.0f, 4.0f);
  // float size = 1.0f;
  // std::array<glm::vec3, 8> vertices;
  // vertices[0] = { center.x - size / 2, center.y - size / 2, center.z + size / 2 };
  // vertices[1] = { center.x + size / 2, center.y - size / 2, center.z + size / 2 };
  // vertices[2] = { center.x + size / 2, center.y + size / 2, center.z + size / 2 };
  // vertices[3] = { center.x - size / 2, center.y + size / 2, center.z + size / 2 };
  //
  // vertices[4] = { center.x + size / 2, center.y + size / 2, center.z - size / 2 };
  // vertices[5] = { center.x - size / 2, center.y + size / 2, center.z - size / 2 };
  // vertices[6] = { center.x - size / 2, center.y - size / 2, center.z - size / 2 };
  // vertices[7] = { center.x + size / 2, center.y - size / 2, center.z - size / 2 };
  //
  // Ancora::Renderer3D::DrawCube(vertices, { 0.2f, 0.3f, 0.7f, 1.0f });
  //
  // Ancora::Renderer3D::EndScene();
}

void CubeMap::OnImGuiRender()
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

void CubeMap::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
