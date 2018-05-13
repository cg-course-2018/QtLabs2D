#include "stdafx.h"
#include "ParticleSystem.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glbinding/gl33core/gl.h>

using glm::vec2;
using glm::vec3;
using glm::vec4;

using namespace gl33core;

namespace
{
// Двумерные координаты двух треугольников, задающих квадратный спрайт.
vec2 SPRITE_VERTECIES[] = {
    { -1.f, -1.f }, // вершина 3
    { +1.f, -1.f }, // вершина 2
    { -1.f, +1.f }, // вершина 0
    { -1.f, +1.f }, // вершина 0
    { +1.f, -1.f }, // вершина 2
    { +1.f, +1.f }, // вершина 1
};

const size_t SPRITE_VERTEX_COUNT = sizeof(SPRITE_VERTECIES) / sizeof(SPRITE_VERTECIES[0]);
}

// Конструирует систему частиц с двумя буферами данных:
//  один используется для статичного спрайта (текстурированного прямоугольника),
//  второй - для постоянно изменяющихся позиций частиц
// в связи с этим у них разные константы BufferUsage.
ParticleSystem::ParticleSystem()
{
    m_spriteGeometry = glcore::createStaticVBO(GL_ARRAY_BUFFER, SPRITE_VERTECIES);
    m_particlePositions = glcore::createVBO();
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::setEmitter(std::unique_ptr<ParticleEmitter> &&pEmitter)
{
    m_pEmitter = std::move(pEmitter);
}

void ParticleSystem::setGravity(const vec3 &gravity)
{
    m_gravity = gravity;
}

void ParticleSystem::setParticleTexture(glcore::TextureObject texture)
{
    m_texture = std::move(texture);
}

void ParticleSystem::update(float deltaSeconds)
{
    // Генерируем новые частицы (за 1 кадр может появиться несколько частиц).
	m_pEmitter->advance(deltaSeconds);
    while (m_pEmitter->isEmitReady())
    {
        m_particles.emplace_back(m_pEmitter->Emit());
    }

    // Продвигаем время жизни всех частиц.
    for (auto &particle : m_particles)
    {
		particle.advance(deltaSeconds, m_gravity);
    }

    // Удаляем "умершие" частицы.
    auto newEnd = std::remove_if(m_particles.begin(), m_particles.end(),
                                 [](const auto &particle) {
        return !particle.isAlive();
    });
    m_particles.erase(newEnd, m_particles.end());

    // Устанавливаем флаг, указывающий, что позиции частиц в видеопамяти
    //  следует обновить.
    m_isDirty = true;

	SceneGraphNode::update(deltaSeconds);
}

void ParticleSystem::draw(const RenderContext& ctx)
{
	const IShaderProgram &program = ctx.program.get();
	const glm::mat4 worldView = ctx.viewMat4 * ctx.worldMat4;

    if (!m_texture)
    {
        throw std::runtime_error("No texture set for particle system");
    }
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Обновляем и привязываем буфер с позициями частиц
	bindParticlePositions(program, worldView);

    // Привязываем буфер с вершинами прямоугольного спрайта.
    glBindBuffer(GL_ARRAY_BUFFER, m_spriteGeometry);
	if (int location = program.getAttribute(AttributeTexCoord); location != -1)
    {
        glEnableVertexAttribArray(static_cast<unsigned>(location));
        glVertexAttribPointer(static_cast<unsigned>(location), glm::vec2().length(), GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);
    }

    const GLsizei vertexCount = GLsizei(SPRITE_VERTEX_COUNT);
    const GLsizei instanceCount = GLsizei(m_particles.size());
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceCount);

	SceneGraphNode::draw(ctx);
}

// Привязывает буфер с позициями частиц,
//  при необходимости обновляет его,
//  затем связывает с атрибутом шейдера.
void ParticleSystem::bindParticlePositions(const IShaderProgram& program, const glm::mat4 &worldView)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_particlePositions);

    if (m_isDirty)
    {
        updateParticlePositions(worldView);
        m_isDirty = false;
    }

    if (int location = program.getAttribute(AttributeInstancePosition); location != -1)
    {
        glEnableVertexAttribArray(static_cast<unsigned>(location));
        glVertexAttribPointer(static_cast<unsigned>(location), glm::vec3().length(), GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);
    }
}

void ParticleSystem::updateParticlePositions(const glm::mat4 &worldView)
{
    // Собираем массив позиций частиц
    std::vector<vec3> positions(m_particles.size());
    std::transform(m_particles.begin(), m_particles.end(),
                   positions.begin(), [](const Particle &particle) {
        return particle.getPosition();
    });

    // Сортируем частицы в порядке удалённости от камеры
    std::sort(positions.begin(), positions.end(), [&](const vec3 &a, vec3 &b) {
        const vec3 viewA = vec3(worldView * vec4(a, 1.0));
        const vec3 viewB = vec3(worldView * vec4(b, 1.0));
        return viewA.z < viewB.z;
    });

    // Отправляем данные на видеокарту
    glcore::setStreamBufferData(m_particlePositions, GL_ARRAY_BUFFER, positions);
}
