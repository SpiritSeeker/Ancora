#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem()
{
  m_ParticlePool.resize(10000);
}

void ParticleSystem::OnUpdate(Ancora::Timestep ts, float gravity)
{
  for (auto& particle : m_ParticlePool)
  {
    if (!particle.Active)
      continue;

    if (particle.LifeRemaining <= 0.0f)
    {
      particle.Active = false;
      continue;
    }

    particle.LifeRemaining -= ts;
    particle.Velocity.y -= gravity * (float)ts;
    particle.Position += particle.Velocity * (float)ts;
    particle.Rotation += 0.01f * ts;
  }
}

void ParticleSystem::OnRender(Ancora::OrthographicCamera& camera)
{
  Ancora::Renderer2D::BeginScene(camera);

  for (auto& particle : m_ParticlePool)
  {
    if (!particle.Active)
      continue;

    float life = particle.LifeRemaining / particle.LifeTime;
    glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
    color.a = color.a * life;

    float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

    Ancora::Renderer2D::DrawRotatedQuad({ particle.Position.x, particle.Position.y, life - 1}, particle.Rotation, { size, size }, color);
  }

  Ancora::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
  Particle& particle = m_ParticlePool[m_PoolIndex];
  particle.Active = true;
  particle.Position = particleProps.Position;
  particle.Rotation = Ancora::Random::Float() * 2.0f * glm::pi<float>();

  particle.Velocity = particleProps.Velocity;
  particle.Velocity.x += particleProps.VelocityVariation.x * (Ancora::Random::Float() - 0.5f);
  particle.Velocity.y += particleProps.VelocityVariation.y * (Ancora::Random::Float() - 0.5f);

  particle.ColorBegin = particleProps.ColorBegin;
  particle.ColorEnd = particleProps.ColorEnd;

  particle.LifeTime = particleProps.LifeTime;
  particle.LifeRemaining = particleProps.LifeTime;
  particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Ancora::Random::Float() - 0.5f);
  particle.SizeEnd = particleProps.SizeEnd;

  m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
