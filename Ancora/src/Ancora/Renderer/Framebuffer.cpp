#include "aepch.h"
#include "Framebuffer.h"

#include "Ancora/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Ancora {

  Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
  {
    switch (Renderer::GetAPI())
    {
      case RendererAPI::API::None:     AE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
      case RendererAPI::API::OpenGL:   return CreateRef<OpenGLFramebuffer>(spec);
    }

    AE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
  }

}
