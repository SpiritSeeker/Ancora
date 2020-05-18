#include "Sandbox2D.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

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
  m_BushTexture = Ancora::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2({ 2, 3 }), glm::vec2({ 128, 128 }));
  Ancora::Random::Init();
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
    Ancora::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_BushTexture);
    Ancora::Renderer2D::EndScene();
  }
}

void Sandbox2D::OnImGuiRender()
{
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
}

void Sandbox2D::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
