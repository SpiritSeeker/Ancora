#include "aepch.h"
#include "Renderer3D.h"

#include "Ancora/Renderer/VertexArray.h"
#include "Ancora/Renderer/Shader.h"
#include "Ancora/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ancora {

  struct Renderer3DStorage
  {
    const uint32_t MaxTriangles = 50000;
    const uint32_t MaxVertices = MaxTriangles * 3;
    const uint32_t MaxIndices = MaxTriangles * 3;

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<IndexBuffer> QuadIndexBuffer;
    ShaderLibrary Shaders;
    Ref<Texture2D> WhiteTexture;
    Ref<Texture2D> ColorTexture;

    uint32_t TriangeIndexCount = 0;
    VertexData3D* VertexBufferBase = nullptr;
    VertexData3D* VertexBufferPtr = nullptr;

    bool EnableCubeMap;
    bool EnableLighting;
    bool ShowLightSource;
  };

  static Renderer3DStorage s_Data;

  void Renderer3D::Init()
  {
    s_Data.QuadVertexArray = VertexArray::Create();

    s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(VertexData3D));
    BufferLayout layout = {
      { ShaderDataType::Float3, "a_Position" },
      { ShaderDataType::Float2, "a_TexCoord" },
      { ShaderDataType::Float3, "a_Normal" }
    };
    s_Data.QuadVertexBuffer->SetLayout(layout);
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

    s_Data.QuadIndexBuffer = IndexBuffer::Create(s_Data.MaxIndices);
    s_Data.QuadVertexArray->SetIndexBuffer(s_Data.QuadIndexBuffer);

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    s_Data.ColorTexture = Texture2D::Create(1, 1);

    s_Data.Shaders.Add(Shader::Create("Sandbox/assets/shaders/FlatColor.glsl"));
    s_Data.Shaders.Add("Texture2D", Shader::Create("Sandbox/assets/shaders/Texture.glsl"));
    s_Data.Shaders.Add(Shader::Create("Sandbox/assets/shaders/CubeMap.glsl"));
    s_Data.Shaders.Add(Shader::Create("Sandbox/assets/shaders/LightCube.glsl"));
    s_Data.Shaders.Add(Shader::Create("Sandbox/assets/shaders/Lighting.glsl"));

    s_Data.Shaders.Get("Texture2D")->Bind();
    s_Data.Shaders.Get("Texture2D")->SetInt("u_Texture", 0);
  }

  void Renderer3D::Shutdown()
  {
  }

  void Renderer3D::BeginScene(const PerspectiveCamera& camera, bool enableCubeMap, bool enableLighting, bool showLightSource)
  {
    s_Data.EnableCubeMap = enableCubeMap;
    s_Data.EnableLighting = enableLighting;
    s_Data.ShowLightSource = showLightSource;

    if (enableCubeMap)
    {
      s_Data.Shaders.Get("CubeMap")->Bind();
      s_Data.Shaders.Get("CubeMap")->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    if (enableLighting)
    {
      s_Data.Shaders.Get("Lighting")->Bind();
      s_Data.Shaders.Get("Lighting")->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
      s_Data.Shaders.Get("Lighting")->SetFloat3("u_CameraPosition", camera.GetPosition());
      s_Data.Shaders.Get("Lighting")->SetFloat3("u_Light.position", camera.GetPosition());
    }
    else
    {
      s_Data.Shaders.Get("Texture2D")->Bind();
      s_Data.Shaders.Get("Texture2D")->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    if (showLightSource)
    {
      s_Data.Shaders.Get("LightCube")->Bind();
      s_Data.Shaders.Get("LightCube")->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }
  }

  void Renderer3D::EndScene()
  {
  }

  void Renderer3D::DrawQuad(const std::array<glm::vec3, 4> &vertices, const glm::vec4& color)
  {
    s_Data.Shaders.Get("Texture2D")->Bind();
    s_Data.Shaders.Get("Texture2D")->SetInt("u_TilingFactor", 1);
    s_Data.Shaders.Get("Texture2D")->SetFloat4("u_Color", color);
    s_Data.WhiteTexture->Bind();

    s_Data.Shaders.Get("Texture2D")->SetMat4("u_Transform", glm::mat4(1.0f));

    glm::vec3 normal = glm::cross(vertices[0] - vertices[1], vertices[1] - vertices[2]);

    float vertexData[] = {
      vertices[0].x, vertices[0].y, vertices[0].z, 0.0f, 0.0f, normal.x, normal.y, normal.z,
      vertices[1].x, vertices[1].y, vertices[1].z, 1.0f, 0.0f, normal.x, normal.y, normal.z,
      vertices[2].x, vertices[2].y, vertices[2].z, 1.0f, 1.0f, normal.x, normal.y, normal.z,
      vertices[2].x, vertices[2].y, vertices[2].z, 1.0f, 1.0f, normal.x, normal.y, normal.z,
      vertices[3].x, vertices[3].y, vertices[3].z, 0.0f, 1.0f, normal.x, normal.y, normal.z,
      vertices[0].x, vertices[0].y, vertices[0].z, 0.0f, 0.0f, normal.x, normal.y, normal.z
    };

    s_Data.QuadVertexBuffer->SetData(vertexData, sizeof(vertexData));

    // TODO: Improve this
    uint32_t indices[] = {
      0, 1, 2,
      2, 3, 0
    };
    s_Data.QuadIndexBuffer->SetData(indices, 6);

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, 6);
  }

  void Renderer3D::DrawCube(const std::array<glm::vec3, 8> &vertices, const glm::vec4& color, const glm::mat4& transform)
  {
    s_Data.Shaders.Get("Texture2D")->Bind();
    s_Data.Shaders.Get("Texture2D")->SetInt("u_TilingFactor", 1);
    s_Data.Shaders.Get("Texture2D")->SetFloat4("u_Color", color);
    s_Data.WhiteTexture->Bind();

    s_Data.Shaders.Get("Texture2D")->SetMat4("u_Transform", transform);

    float vertexData[] = {
      // -x
      vertices[6].x, vertices[6].y, vertices[6].z, 0.0f, 0.0f, (vertices[6] - vertices[7]).x, (vertices[6] - vertices[7]).y, (vertices[6] - vertices[7]).z,
      vertices[0].x, vertices[0].y, vertices[0].z, 1.0f, 0.0f, (vertices[6] - vertices[7]).x, (vertices[6] - vertices[7]).y, (vertices[6] - vertices[7]).z,
      vertices[3].x, vertices[3].y, vertices[3].z, 1.0f, 1.0f, (vertices[6] - vertices[7]).x, (vertices[6] - vertices[7]).y, (vertices[6] - vertices[7]).z,
      vertices[3].x, vertices[3].y, vertices[3].z, 1.0f, 1.0f, (vertices[6] - vertices[7]).x, (vertices[6] - vertices[7]).y, (vertices[6] - vertices[7]).z,
      vertices[5].x, vertices[5].y, vertices[5].z, 0.0f, 1.0f, (vertices[6] - vertices[7]).x, (vertices[6] - vertices[7]).y, (vertices[6] - vertices[7]).z,
      vertices[6].x, vertices[6].y, vertices[6].z, 0.0f, 0.0f, (vertices[6] - vertices[7]).x, (vertices[6] - vertices[7]).y, (vertices[6] - vertices[7]).z,

      // +x
      vertices[2].x, vertices[2].y, vertices[2].z, 0.0f, 0.0f, (vertices[7] - vertices[6]).x, (vertices[7] - vertices[6]).y, (vertices[7] - vertices[6]).z,
      vertices[4].x, vertices[4].y, vertices[4].z, 1.0f, 0.0f, (vertices[7] - vertices[6]).x, (vertices[7] - vertices[6]).y, (vertices[7] - vertices[6]).z,
      vertices[7].x, vertices[7].y, vertices[7].z, 1.0f, 1.0f, (vertices[7] - vertices[6]).x, (vertices[7] - vertices[6]).y, (vertices[7] - vertices[6]).z,
      vertices[7].x, vertices[7].y, vertices[7].z, 1.0f, 1.0f, (vertices[7] - vertices[6]).x, (vertices[7] - vertices[6]).y, (vertices[7] - vertices[6]).z,
      vertices[1].x, vertices[1].y, vertices[1].z, 0.0f, 1.0f, (vertices[7] - vertices[6]).x, (vertices[7] - vertices[6]).y, (vertices[7] - vertices[6]).z,
      vertices[2].x, vertices[2].y, vertices[2].z, 0.0f, 0.0f, (vertices[7] - vertices[6]).x, (vertices[7] - vertices[6]).y, (vertices[7] - vertices[6]).z,

      // -y
      vertices[6].x, vertices[6].y, vertices[6].z, 0.0f, 0.0f, (vertices[0] - vertices[3]).x, (vertices[0] - vertices[3]).y, (vertices[0] - vertices[3]).z,
      vertices[7].x, vertices[7].y, vertices[7].z, 1.0f, 0.0f, (vertices[0] - vertices[3]).x, (vertices[0] - vertices[3]).y, (vertices[0] - vertices[3]).z,
      vertices[1].x, vertices[1].y, vertices[1].z, 1.0f, 1.0f, (vertices[0] - vertices[3]).x, (vertices[0] - vertices[3]).y, (vertices[0] - vertices[3]).z,
      vertices[1].x, vertices[1].y, vertices[1].z, 1.0f, 1.0f, (vertices[0] - vertices[3]).x, (vertices[0] - vertices[3]).y, (vertices[0] - vertices[3]).z,
      vertices[0].x, vertices[0].y, vertices[0].z, 0.0f, 1.0f, (vertices[0] - vertices[3]).x, (vertices[0] - vertices[3]).y, (vertices[0] - vertices[3]).z,
      vertices[6].x, vertices[6].y, vertices[6].z, 0.0f, 0.0f, (vertices[0] - vertices[3]).x, (vertices[0] - vertices[3]).y, (vertices[0] - vertices[3]).z,

      // +y
      vertices[2].x, vertices[2].y, vertices[2].z, 0.0f, 0.0f, (vertices[3] - vertices[0]).x, (vertices[3] - vertices[0]).y, (vertices[3] - vertices[0]).z,
      vertices[3].x, vertices[3].y, vertices[3].z, 1.0f, 0.0f, (vertices[3] - vertices[0]).x, (vertices[3] - vertices[0]).y, (vertices[3] - vertices[0]).z,
      vertices[5].x, vertices[5].y, vertices[5].z, 1.0f, 1.0f, (vertices[3] - vertices[0]).x, (vertices[3] - vertices[0]).y, (vertices[3] - vertices[0]).z,
      vertices[5].x, vertices[5].y, vertices[5].z, 1.0f, 1.0f, (vertices[3] - vertices[0]).x, (vertices[3] - vertices[0]).y, (vertices[3] - vertices[0]).z,
      vertices[4].x, vertices[4].y, vertices[4].z, 0.0f, 1.0f, (vertices[3] - vertices[0]).x, (vertices[3] - vertices[0]).y, (vertices[3] - vertices[0]).z,
      vertices[2].x, vertices[2].y, vertices[2].z, 0.0f, 0.0f, (vertices[3] - vertices[0]).x, (vertices[3] - vertices[0]).y, (vertices[3] - vertices[0]).z,

      // -z
      vertices[0].x, vertices[0].y, vertices[0].z, 0.0f, 0.0f, (vertices[0] - vertices[6]).x, (vertices[0] - vertices[6]).y, (vertices[0] - vertices[6]).z,
      vertices[1].x, vertices[1].y, vertices[1].z, 1.0f, 0.0f, (vertices[0] - vertices[6]).x, (vertices[0] - vertices[6]).y, (vertices[0] - vertices[6]).z,
      vertices[2].x, vertices[2].y, vertices[2].z, 1.0f, 1.0f, (vertices[0] - vertices[6]).x, (vertices[0] - vertices[6]).y, (vertices[0] - vertices[6]).z,
      vertices[2].x, vertices[2].y, vertices[2].z, 1.0f, 1.0f, (vertices[0] - vertices[6]).x, (vertices[0] - vertices[6]).y, (vertices[0] - vertices[6]).z,
      vertices[3].x, vertices[3].y, vertices[3].z, 0.0f, 1.0f, (vertices[0] - vertices[6]).x, (vertices[0] - vertices[6]).y, (vertices[0] - vertices[6]).z,
      vertices[0].x, vertices[0].y, vertices[0].z, 0.0f, 0.0f, (vertices[0] - vertices[6]).x, (vertices[0] - vertices[6]).y, (vertices[0] - vertices[6]).z,

      // +z
      vertices[4].x, vertices[4].y, vertices[4].z, 0.0f, 0.0f, (vertices[6] - vertices[0]).x, (vertices[6] - vertices[0]).y, (vertices[6] - vertices[0]).z,
      vertices[5].x, vertices[5].y, vertices[5].z, 1.0f, 0.0f, (vertices[6] - vertices[0]).x, (vertices[6] - vertices[0]).y, (vertices[6] - vertices[0]).z,
      vertices[6].x, vertices[6].y, vertices[6].z, 1.0f, 1.0f, (vertices[6] - vertices[0]).x, (vertices[6] - vertices[0]).y, (vertices[6] - vertices[0]).z,
      vertices[6].x, vertices[6].y, vertices[6].z, 1.0f, 1.0f, (vertices[6] - vertices[0]).x, (vertices[6] - vertices[0]).y, (vertices[6] - vertices[0]).z,
      vertices[7].x, vertices[7].y, vertices[7].z, 0.0f, 1.0f, (vertices[6] - vertices[0]).x, (vertices[6] - vertices[0]).y, (vertices[6] - vertices[0]).z,
      vertices[4].x, vertices[4].y, vertices[4].z, 0.0f, 0.0f, (vertices[6] - vertices[0]).x, (vertices[6] - vertices[0]).y, (vertices[6] - vertices[0]).z
    };

    s_Data.QuadVertexBuffer->SetData(vertexData, sizeof(vertexData));

    uint32_t indices[36];
    // TODO: Improve this
    for (uint32_t i = 0; i < 36; i++)
      indices[i] = i;
    s_Data.QuadIndexBuffer->SetData(indices, 36);

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, 36);
  }

  void Renderer3D::DrawObject(const std::vector<VertexData3D>& vertexData, const glm::vec4& color, const glm::mat4& transform)
  {
    if (s_Data.EnableLighting)
    {
      s_Data.Shaders.Get("Lighting")->Bind();
      s_Data.WhiteTexture->Bind(0);

      uint32_t colorTextureData = 0;
      for (int i = 0; i < 4; i++)
      {
        colorTextureData *= 256;
        colorTextureData += (int)(color[3-i] * 255);
      }
      s_Data.ColorTexture->SetData(&colorTextureData, sizeof(uint32_t));
      s_Data.ColorTexture->Bind(1);

      s_Data.Shaders.Get("Lighting")->SetMat4("u_Transform", transform);

      s_Data.Shaders.Get("Lighting")->SetInt("u_Material.diffuse", 1);
      s_Data.Shaders.Get("Lighting")->SetInt("u_Material.specular", 0);
      s_Data.Shaders.Get("Lighting")->SetFloat("u_Material.shininess", 32.0f);

      s_Data.Shaders.Get("Lighting")->SetFloat3("u_Light.position", { -2.0f, 2.0f, 2.0f });
      s_Data.Shaders.Get("Lighting")->SetFloat3("u_Light.ambient", glm::vec3(0.2f));
      s_Data.Shaders.Get("Lighting")->SetFloat3("u_Light.diffuse", glm::vec3(0.5f));
      s_Data.Shaders.Get("Lighting")->SetFloat3("u_Light.specular", glm::vec3(1.0f));
    }

    else
    {
      s_Data.Shaders.Get("Texture2D")->SetInt("u_TilingFactor", 1);
      s_Data.Shaders.Get("Texture2D")->SetFloat4("u_Color", color);
      s_Data.WhiteTexture->Bind();

      s_Data.Shaders.Get("Texture2D")->SetMat4("u_Transform", transform);
    }

    if (vertexData.size() > s_Data.MaxVertices)
    {
      uint32_t count = vertexData.size();
      uint32_t numIter = 0;
      while (count > s_Data.MaxVertices)
      {
        std::vector<VertexData3D> data(vertexData.begin() + numIter * s_Data.MaxVertices, vertexData.begin() + (numIter + 1) * s_Data.MaxVertices);
        numIter++;
        count -= s_Data.MaxVertices;
        s_Data.QuadVertexBuffer->SetData(&data[0], s_Data.MaxVertices * sizeof(VertexData3D));

        uint32_t* indices = new uint32_t[s_Data.MaxIndices];
        for (uint32_t i = 0; i < s_Data.MaxIndices; i++)
          indices[i] = i;
        s_Data.QuadIndexBuffer->SetData(indices, s_Data.MaxIndices);
        delete[] indices;

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.MaxIndices);
      }

      std::vector<VertexData3D> data(vertexData.begin() + numIter * s_Data.MaxVertices, vertexData.end());
      s_Data.QuadVertexBuffer->SetData(&data[0], count * sizeof(VertexData3D));

      uint32_t* indices = new uint32_t[count];
      for (uint32_t i = 0; i < count; i++)
        indices[i] = i;
      s_Data.QuadIndexBuffer->SetData(indices, count);
      delete[] indices;

      RenderCommand::DrawIndexed(s_Data.QuadVertexArray, count);
    }

    else
    {
      s_Data.QuadVertexBuffer->SetData(&vertexData[0], vertexData.size() * sizeof(VertexData3D));

      uint32_t* indices = new uint32_t[vertexData.size()];
      for (uint32_t i = 0; i < vertexData.size(); i++)
        indices[i] = i;
      s_Data.QuadIndexBuffer->SetData(indices, vertexData.size());
      delete[] indices;

      RenderCommand::DrawIndexed(s_Data.QuadVertexArray, vertexData.size());
    }
  }

  void Renderer3D::DrawModel(Ref<Model3D> model, const glm::mat4& transform)
  {
    s_Data.Shaders.Get("Lighting")->Bind();
    s_Data.Shaders.Get("Lighting")->SetMat4("u_Transform", transform);

    s_Data.Shaders.Get("Lighting")->SetInt("u_Material.diffuse", 0);
    s_Data.Shaders.Get("Lighting")->SetInt("u_Material.specular", 1);
    s_Data.Shaders.Get("Lighting")->SetFloat("u_Material.shininess", 32.0f);

    // s_Data.Shaders.Get("Lighting")->SetFloat3("u_Light.position", { -5.0f, 5.0f, 5.0f });
    s_Data.Shaders.Get("Lighting")->SetFloat3("u_Light.ambient", glm::vec3(0.2f));
    s_Data.Shaders.Get("Lighting")->SetFloat3("u_Light.diffuse", glm::vec3(0.5f));
    s_Data.Shaders.Get("Lighting")->SetFloat3("u_Light.specular", glm::vec3(1.0f));

    for (auto& mesh : model->GetMesh())
    {
      for (uint32_t i = 0; i < mesh.DiffuseTextures.size(); i++)
        mesh.DiffuseTextures[i]->Bind(0);
        for (uint32_t i = 0; i < mesh.SpecularTextures.size(); i++)
          mesh.SpecularTextures[i]->Bind(1);

      if (mesh.Vertices.size() > s_Data.MaxVertices)
      {
        uint32_t count = mesh.Vertices.size();
        uint32_t numIter = 0;
        while (count > s_Data.MaxVertices)
        {
          std::vector<VertexData3D> data(mesh.Vertices.begin() + numIter * s_Data.MaxVertices, mesh.Vertices.begin() + (numIter + 1) * s_Data.MaxVertices);
          std::vector<uint32_t> index(mesh.Indices.begin() + numIter * s_Data.MaxIndices, mesh.Indices.begin() + (numIter + 1) * s_Data.MaxIndices);
          numIter++;
          count -= s_Data.MaxVertices;
          s_Data.QuadVertexBuffer->SetData(&data[0], s_Data.MaxVertices * sizeof(VertexData3D));
          s_Data.QuadIndexBuffer->SetData(&index[0], s_Data.MaxIndices);
          RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.MaxIndices);
        }

        std::vector<VertexData3D> data(mesh.Vertices.begin() + numIter * s_Data.MaxVertices, mesh.Vertices.end());
        std::vector<uint32_t> index(mesh.Indices.begin() + numIter * s_Data.MaxIndices, mesh.Indices.end());
        s_Data.QuadVertexBuffer->SetData(&data[0], count * sizeof(VertexData3D));
        s_Data.QuadIndexBuffer->SetData(&index[0], count);
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, count);
      }

      else
      {
        s_Data.QuadVertexBuffer->SetData(&mesh.Vertices[0], mesh.Vertices.size() * sizeof(VertexData3D));
        s_Data.QuadIndexBuffer->SetData(&mesh.Indices[0], mesh.Indices.size());
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, mesh.Indices.size());
      }
    }
  }

}
