#include "Sandbox2D.h"

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

// Rework this mess.
void GetProfileResults(Sandbox2D* sandbox2D, const char* name, float duration)
{
  Sandbox2D::ProfileResult pr = { name, duration };
  sandbox2D->m_ProfileResults.push_back(pr);
}

class Timer
{
public:
  template<typename Fn>
  Timer(const char* name, Fn& func, Sandbox2D* sandbox2D)
    : m_Name(name), m_Func(func), m_Stopped(false), m_sb(sandbox2D)
  {
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
  }

  ~Timer()
  {
    if (!m_Stopped)
      Stop();
  }

  void Stop()
  {
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

    m_Stopped = true;

    float duration = (end - start) * 0.001f;
    m_Func(m_sb, m_Name, duration);
  }
private:
  const char* m_Name;
  void (*m_Func)(Sandbox2D*, const char*, float);
  Sandbox2D* m_sb;
  bool m_Stopped;
  std::chrono::time_point<std::chrono::system_clock> m_StartTimepoint;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, GetProfileResults, this)

Sandbox2D::Sandbox2D()
  : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
  m_Texture = Ancora::Texture2D::Create("Sandbox/assets/textures/pic.png");
  m_SpriteSheet = Ancora::Texture2D::Create("Sandbox/assets/game/textures/RPGpack_sheet_2X.png");

  m_MapWidth = s_MapWidth;
  m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

  m_TextureMap['D'] = Ancora::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 1, 11 }), glm::vec2({ 128, 128 }));
  m_TextureMap['W'] = Ancora::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 11, 11 }), glm::vec2({ 128, 128 }));
  m_TextureMap['B'] = Ancora::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 2, 3 }), glm::vec2({ 128, 128 }));
  m_TextureMap['t'] = Ancora::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 4, 2 }), glm::vec2({ 128, 128 }));
  m_TextureMap['T'] = Ancora::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 4, 1 }), glm::vec2({ 128, 128 }));

  Ancora::Random::Init();

  m_CameraController.SetZoomLevel(4.0f);
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Ancora::Timestep ts)
{
  PROFILE_SCOPE("Sandbox2D::OnUpdate");

  // Update
	m_FPS = 1 / ts;

  {
    PROFILE_SCOPE("CameraController::OnUpdate");
	  m_CameraController.OnUpdate(ts);
  }

	// Render
  {
    PROFILE_SCOPE("Renderer Prep");
    Ancora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	  Ancora::RenderCommand::Clear();
  }

  {
    PROFILE_SCOPE("Renderer Draw");
  	Ancora::Renderer2D::BeginScene(m_CameraController.GetCamera());
    // Ancora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    // Ancora::Renderer2D::DrawRotatedQuad({ 1.0f, -0.5f }, glm::radians(30.0f), { 0.15f, 0.35f }, m_SquareColor);
    // Ancora::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_BushTexture);

    for (uint32_t y = 0; y < m_MapHeight; y++)
    {
      for (uint32_t x = 0; x < m_MapWidth; x++)
      {
        char tileType = s_MapTiles[x + y * m_MapWidth];
        Ancora::Renderer2D::DrawQuad({ x - (m_MapWidth / 2.0f), (m_MapHeight / 2.0f) - y }, { 1.0f, 1.0f }, m_TextureMap[tileType]);
      }
    }

    for (uint32_t y = 0; y < m_MapHeight; y++)
    {
      for (uint32_t x = 0; x < m_MapWidth; x++)
      {
        char tileType = s_MapTrees[x + y * m_MapWidth];
        Ancora::Renderer2D::DrawQuad({ x - (m_MapWidth / 2.0f), (m_MapHeight / 2.0f) - y, 0.5f }, { 1.0f, 1.0f }, m_TextureMap[tileType]);
      }
    }

    Ancora::Renderer2D::EndScene();
  }
}

void Sandbox2D::OnImGuiRender()
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

          if (ImGui::MenuItem("Exit")) Ancora::Application::Get().Close();
          ImGui::EndMenu();
      }

      ImGui::EndMenuBar();
  }

  ImGui::Begin("Test");
	ImGui::Text("FPS: %d, %f", m_FPS, Ancora::Random::Float());
	ImGui::End();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

  for (auto& result : m_ProfileResults)
  {
    char label[50];
    strcpy(label, "%.3fms ");
    strcat(label, result.Name);
    ImGui::Text(label, result.Time);
  }
  m_ProfileResults.clear();
  ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
