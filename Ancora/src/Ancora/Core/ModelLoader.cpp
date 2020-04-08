#include "aepch.h"
#include "ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Ancora {

  Ref<Model3D> ModelLoader::LoadModel(const std::string& filename)
  {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    AE_CORE_ASSERT(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode, "Assimp Load Error: {0}", importer.GetErrorString());

    std::string currentDirectory = filename.substr(0, filename.find_last_of('/'));
    std::string modelName = filename.substr(filename.find_last_of('/') + 1, filename.find_last_of('.'));

    Ref<Model3D> outModel = CreateRef<Model3D>();
    outModel->SetName(modelName);

    ProcessNode(scene->mRootNode, scene, outModel, currentDirectory);
    return outModel;
  }

  void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Ref<Model3D> model, const std::string& currentDirectory)
  {
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      model->AddMesh(ProcessMesh(mesh, scene, currentDirectory));
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
      ProcessNode(node->mChildren[i], scene, model, currentDirectory);
    }
  }

  Mesh ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& currentDirectory)
  {
    Mesh outMesh;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
      VertexData3D vertex;

      vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
      vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
      // Currently supports only 1 set of texture coordinates per vertex
      if (mesh->mTextureCoords[0])
        vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
      else
        vertex.TexCoord = { 0.0f, 0.0f };

      outMesh.Vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
      aiFace face = mesh->mFaces[i];
      for (uint32_t j = 0; j < face.mNumIndices; j++)
        outMesh.Indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      // AE_CORE_TRACE("Model Texture Info:");
      // AE_CORE_TRACE("  Diffuse:      {0}", material->GetTextureCount(aiTextureType_DIFFUSE));
      // AE_CORE_TRACE("  Specular:     {0}", material->GetTextureCount(aiTextureType_SPECULAR));
      // AE_CORE_TRACE("  Ambient:      {0}", material->GetTextureCount(aiTextureType_AMBIENT));
      // AE_CORE_TRACE("  Emissive:     {0}", material->GetTextureCount(aiTextureType_EMISSIVE));
      // AE_CORE_TRACE("  Height:       {0}", material->GetTextureCount(aiTextureType_HEIGHT));
      // AE_CORE_TRACE("  Normals:      {0}", material->GetTextureCount(aiTextureType_NORMALS));
      // AE_CORE_TRACE("  Shininess:    {0}", material->GetTextureCount(aiTextureType_SHININESS));
      // AE_CORE_TRACE("  Opacity:      {0}", material->GetTextureCount(aiTextureType_OPACITY));
      // AE_CORE_TRACE("  Displacement: {0}", material->GetTextureCount(aiTextureType_DISPLACEMENT));
      // AE_CORE_TRACE("  Lightmap:     {0}", material->GetTextureCount(aiTextureType_LIGHTMAP));
      // AE_CORE_TRACE("  Reflection:   {0}", material->GetTextureCount(aiTextureType_REFLECTION));
      // AE_CORE_TRACE("  Unknown:      {0}", material->GetTextureCount(aiTextureType_UNKNOWN));

      LoadMaterialTexture(material, aiTextureType_DIFFUSE, outMesh.DiffuseTextures, currentDirectory, outMesh);
      LoadMaterialTexture(material, aiTextureType_SPECULAR, outMesh.SpecularTextures, currentDirectory, outMesh);
    }

    return outMesh;
  }

  void ModelLoader::LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::vector<Ref<Texture2D>>& textures, const std::string& currentDirectory, const Mesh& mesh)
  {
    for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
    {
      aiString str;
      material->GetTexture(type, i, &str);
      std::string filePath = std::string(str.C_Str());
      std::replace(filePath.begin(), filePath.end(), '\\', '/');
      std::string texPath = currentDirectory + '/' + filePath;
      AE_CORE_TRACE("{0}", texPath);

      bool present = false;
      switch (type)
      {
        case aiTextureType_DIFFUSE:
        {
          for (auto& tex : mesh.DiffuseTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_SPECULAR:
        {
          for (auto& tex : mesh.SpecularTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_AMBIENT:
        {
          for (auto& tex : mesh.AmbientTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_EMISSIVE:
        {
          for (auto& tex : mesh.EmissiveTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_HEIGHT:
        {
          for (auto& tex : mesh.HeightTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_NORMALS:
        {
          for (auto& tex : mesh.NormalsTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_SHININESS:
        {
          for (auto& tex : mesh.ShininessTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_OPACITY:
        {
          for (auto& tex : mesh.OpacityTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_DISPLACEMENT:
        {
          for (auto& tex : mesh.DisplacementTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_LIGHTMAP:
        {
          for (auto& tex : mesh.LightmapTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_REFLECTION:
        {
          for (auto& tex : mesh.ReflectionTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }

        case aiTextureType_UNKNOWN:
        {
          for (auto& tex : mesh.UnknownTextures)
          {
            if (tex->GetName() == texPath)
            {
              textures.push_back(tex);
              present = true;
              break;
            }
          }
        }
      }

      if (!present)
      {
        Ref<Texture2D> texture = Texture2D::Create(texPath);
        textures.push_back(texture);
      }
    }
  }

}
