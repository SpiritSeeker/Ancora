#include "aepch.h"
#include "Renderer3D.h"

#include "Ancora/Renderer/VertexArray.h"
#include "Ancora/Renderer/Shader.h"
#include "Ancora/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ancora {

  struct Renderer3DStorage
  {
    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;

    glm::mat4 ViewProjection;
  };

  static Renderer3DStorage s_Data;

  void Renderer3D::Init()
  {
    s_Data.QuadVertexArray = VertexArray::Create();

    s_Data.QuadVertexBuffer = VertexBuffer::Create(8 * 9 * sizeof(float));
    BufferLayout layout = {
      { ShaderDataType::Float3, "a_Position" },
      { ShaderDataType::Float2, "a_TexCoord" },
      { ShaderDataType::Float4, "a_Color" }
    };
    s_Data.QuadVertexBuffer->SetLayout(layout);
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

    uint32_t indices[] = {
      0, 1, 2,
      2, 3, 0,
      1, 7, 4,
      4, 2, 1,
      7, 6, 5,
      5, 4, 7,
      6, 0, 3,
      3, 5, 6,
      2, 3, 5,
      5, 4, 2,
      1, 0, 6,
      6, 7, 1
    };

    Ref<IndexBuffer> indexBuffer;
    indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    s_Data.TextureShader = Shader::Create("Sandbox/assets/shaders/Texture.glsl");
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetInt("u_Texture", 0);
  }

  void Renderer3D::Shutdown()
  {
  }

  void Renderer3D::BeginScene(const PerspectiveCamera& camera)
  {
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    s_Data.ViewProjection = camera.GetViewProjectionMatrix();
  }

  void Renderer3D::EndScene()
  {
  }

  void Renderer3D::DrawQuad(const std::array<glm::vec3, 4> &vertices, const glm::vec4& color)
  {
    s_Data.TextureShader->SetInt("u_TilingFactor", 1);
    s_Data.TextureShader->SetFloat4("u_Color", color);
    s_Data.WhiteTexture->Bind();

    s_Data.TextureShader->SetMat4("u_Transform", glm::mat4(1.0f));

    float vertexData[] = {
      vertices[0].x, vertices[0].y, vertices[0].z, 0.0f, 0.0f, color.r, color.g, color.b, color.a,
      vertices[1].x, vertices[1].y, vertices[1].z, 1.0f, 0.0f, color.r, color.g, color.b, color.a,
      vertices[2].x, vertices[2].y, vertices[2].z, 1.0f, 1.0f, color.r, color.g, color.b, color.a,
      vertices[3].x, vertices[3].y, vertices[3].z, 0.0f, 1.0f, color.r, color.g, color.b, color.a
    };

    s_Data.QuadVertexBuffer->SetData(vertexData, sizeof(vertexData));

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, 6);
  }

  void Renderer3D::DrawCube(const std::array<glm::vec3, 8> &vertices, const glm::vec4& color, const glm::mat4& transform)
  {
    s_Data.TextureShader->SetInt("u_TilingFactor", 1);
    s_Data.TextureShader->SetFloat4("u_Color", color);
    s_Data.WhiteTexture->Bind();

    s_Data.TextureShader->SetMat4("u_Transform", transform);

    float vertexData[] = {
      vertices[0].x, vertices[0].y, vertices[0].z, 0.0f, 0.0f, color.r, color.g, color.b, color.a,
      vertices[1].x, vertices[1].y, vertices[1].z, 1.0f, 0.0f, color.r, color.g, color.b, color.a,
      vertices[2].x, vertices[2].y, vertices[2].z, 1.0f, 1.0f, color.r, color.g, color.b, color.a,
      vertices[3].x, vertices[3].y, vertices[3].z, 0.0f, 1.0f, color.r, color.g, color.b, color.a,

      vertices[4].x, vertices[4].y, vertices[4].z, 0.0f, 0.0f, 1 - color.r, color.g, color.b, color.a,
      vertices[5].x, vertices[5].y, vertices[5].z, 1.0f, 0.0f, 1 - color.r, color.g, color.b, color.a,
      vertices[6].x, vertices[6].y, vertices[6].z, 1.0f, 1.0f, 1 - color.r, color.g, color.b, color.a,
      vertices[7].x, vertices[7].y, vertices[7].z, 0.0f, 1.0f, 1 - color.r, color.g, color.b, color.a
    };

    s_Data.QuadVertexBuffer->SetData(vertexData, sizeof(vertexData));

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
  }

}
