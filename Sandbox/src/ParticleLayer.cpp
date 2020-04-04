#include "ParticleLayer.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ParticleLayer::ParticleLayer()
  : Layer("ParticleLayer"), m_CameraController(1280.0f / 720.0f)
{
}

void ParticleLayer::OnAttach()
{
  Ancora::Random::Init();

  m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
  m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
  m_Particle.SizeBegin = 0.2f, m_Particle.SizeVariation = 0.1f, m_Particle.SizeEnd = 0.0f;
  m_Particle.LifeTime = 1.0f;
  m_Particle.Velocity = { -5.0f, 0.0f };
  m_Particle.VelocityVariation = { 3.0f, 1.0f };
  m_Particle.Position = { 0.0f, 0.0f };
}

void ParticleLayer::OnDetach()
{
}

void ParticleLayer::OnUpdate(Ancora::Timestep ts)
{
  m_FPS = 1 / ts;
  m_CameraController.OnUpdate(ts);

  Ancora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
  Ancora::RenderCommand::Clear();

  if (Ancora::Input::IsMouseButtonPressed(AE_MOUSE_BUTTON_LEFT))
  {
    auto mousePos = Ancora::Input::GetMousePosition();
    auto width = Ancora::Application::Get().GetWindow().GetWidth();
    auto height = Ancora::Application::Get().GetWindow().GetHeight();

    auto bounds = m_CameraController.GetBounds();
    auto pos = m_CameraController.GetCamera().GetPosition();
    auto x = (mousePos.first / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
    auto y = bounds.GetHeight() * 0.5f - (mousePos.second / height) * bounds.GetHeight();
    m_Particle.Position = { x + pos.x, y + pos.y };
    for (int i = 0; i < m_Number; i++)
      m_ParticleSystem.Emit(m_Particle);
  }

  m_ParticleSystem.OnUpdate(ts, m_Gravity);
  m_ParticleSystem.OnRender(m_CameraController.GetCamera());
}

void ParticleLayer::OnImGuiRender()
{
  ImGui::Begin("Metrics");
  ImGui::Text("FPS: %d", m_FPS);
  ImGui::End();

  ImGui::Begin("Color Palette");
  ImGui::ColorEdit4("Birth color", glm::value_ptr(m_Particle.ColorBegin));
  ImGui::ColorEdit4("Death color", glm::value_ptr(m_Particle.ColorEnd));
  ImGui::DragFloat("LifeTime", &m_Particle.LifeTime, 0.1f, 0.0f, 1000.0f);
  ImGui::DragInt("Number", &m_Number, 1, 2, 50);
  ImGui::DragFloat("Gravity", &m_Gravity, 0.1f, 0.0f, 15.0f);
  ImGui::End();
}

void ParticleLayer::OnEvent(Ancora::Event& e)
{
  m_CameraController.OnEvent(e);
}
