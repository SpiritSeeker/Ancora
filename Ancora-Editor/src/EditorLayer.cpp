#include "EditorLayer.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

static const uint32_t s_MapWidth = 16;
static const char* s_MapTiles =
"WWWWWWWWWWWWWWWW"
"WWWWWWDDDDWWWWWW"
"WWWWWDDDDDDWWWWW"
"WWWWDDDDDDDDWWWW"
"WWWDDDDDDDDDWWWW"
"WWWDDDDDDDDDDWWW"
"WWDDWWWDDDDDDWWW"
"WDDDWWWDDDDDDDWW"
"WDDDWWWDDDDDDDWW"
"WWDDDDDDDDDDDWWW"
"WWWDDDDDDDDDDWWW"
"WWWDDDDDDDDDWWWW"
"WWWWDDDDDDDDWWWW"
"WWWWWDDDDDDWWWWW"
"WWWWWWDDDDWWWWWW"
"WWWWWWWWWWWWWWWW"
;

static const char* s_MapTrees =
"WWWWWWWWWWWWWWWW"
"WWWWWWDDDDWWWWWW"
"WWWWWDDDDDDWWWWW"
"WWWWDDDDDDDDWWWW"
"WWWDDDDDDDDDWWWW"
"WWWDDDDDDDDDDWWW"
"WWDDWWWDDDDDDWWW"
"WDDDWWWDDDDDDDWW"
"WDDDWWWDDDtDDDWW"
"WWDDDDDDDBTDDWWW"
"WWWDDDDDDDDDDWWW"
"WWWDDDDDDDDDWWWW"
"WWWWDDDDDDDDWWWW"
"WWWWWDDDDDDWWWWW"
"WWWWWWDDDDWWWWWW"
"WWWWWWWWWWWWWWWW"
;

namespace Ancora {

  EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
  {
  }

  void EditorLayer::OnAttach()
  {
    m_Texture = Texture2D::Create("Sandbox/assets/textures/pic.png");
    m_SpriteSheet = Texture2D::Create("Sandbox/assets/game/textures/RPGpack_sheet_2X.png");

    m_MapWidth = s_MapWidth;
    m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

    m_TextureMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 1, 11 }), glm::vec2({ 128, 128 }));
    m_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 11, 11 }), glm::vec2({ 128, 128 }));
    m_TextureMap['B'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 2, 3 }), glm::vec2({ 128, 128 }));
    m_TextureMap['t'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 4, 2 }), glm::vec2({ 128, 128 }));
    m_TextureMap['T'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 4, 1 }), glm::vec2({ 128, 128 }));

    Random::Init();

    m_CameraController.SetZoomLevel(4.0f);

    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);
  }

  void EditorLayer::OnDetach()
  {
  }

  void EditorLayer::OnUpdate(Timestep ts)
  {
    // Update
  	m_FPS = 1 / ts;

	  m_CameraController.OnUpdate(ts);

  	// Render
    m_Framebuffer->Bind();
    RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	  RenderCommand::Clear();

  	Renderer2D::BeginScene(m_CameraController.GetCamera());

    for (uint32_t y = 0; y < m_MapHeight; y++)
    {
      for (uint32_t x = 0; x < m_MapWidth; x++)
      {
        char tileType = s_MapTiles[x + y * m_MapWidth];
        Renderer2D::DrawQuad({ x - (m_MapWidth / 2.0f), (m_MapHeight / 2.0f) - y }, { 1.0f, 1.0f }, m_TextureMap[tileType]);
      }
    }

    for (uint32_t y = 0; y < m_MapHeight; y++)
    {
      for (uint32_t x = 0; x < m_MapWidth; x++)
      {
        char tileType = s_MapTrees[x + y * m_MapWidth];
        Renderer2D::DrawQuad({ x - (m_MapWidth / 2.0f), (m_MapHeight / 2.0f) - y, 0.5f }, { 1.0f, 1.0f }, m_TextureMap[tileType]);
      }
    }

    Renderer2D::EndScene();
    m_Framebuffer->Unbind();
  }

  void EditorLayer::OnImGuiRender()
  {
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Exit")) Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Test");
  	ImGui::Text("FPS: %d, %f", m_FPS, Random::Float());
  	ImGui::End();

  	ImGui::Begin("Settings");
  	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ 1080.0f, 720.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();

  	ImGui::End();
  }

  void EditorLayer::OnEvent(Event& e)
  {
    m_CameraController.OnEvent(e);
  }

}
