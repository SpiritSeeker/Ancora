#pragma once

#include "Ancora/Renderer/Texture.h"

#include <glad/glad.h>

namespace Ancora {

  class OpenGLTexture2D : public Texture2D
  {
  public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    OpenGLTexture2D(const std::string& path);
    virtual ~OpenGLTexture2D();

    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override { return m_Height; }
    virtual std::string GetName() const override { return m_Path; }

    virtual void SetData(void* data, uint32_t size) override;

    virtual void Bind(uint32_t slot = 0) const override;
  private:
    std::string m_Path;
    uint32_t m_Width, m_Height;
    uint32_t m_RendererID;
    GLenum m_InternalFormat, m_DataFormat;
  };

  class OpenGLCubeMap : public CubeMap
  {
  public:
    OpenGLCubeMap(const std::array<std::string, 6>& cubePaths);
    virtual ~OpenGLCubeMap();

    virtual uint32_t GetSize() const override { return m_Size; }

    virtual void Bind(uint32_t slot = 0) const override;
  private:
    uint32_t m_Size;
    uint32_t m_RendererID;
    GLenum m_InternalFormat, m_DataFormat;
  };

}
