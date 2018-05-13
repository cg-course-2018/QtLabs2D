#pragma once

#include <glm/vec3.hpp>
#include "ParticleEmitter.h"
#include "IShaderProgram.h"
#include "libglcore/libglcore.h"
#include "SceneGraph.h"

class ParticleSystem : public SceneGraphNode
{
public:
	ParticleSystem();
    ~ParticleSystem();

    ParticleSystem(const ParticleSystem&) = delete;
    ParticleSystem& operator=(const ParticleSystem&) = delete;

    void setEmitter(std::unique_ptr<ParticleEmitter> && pEmitter);
    void setGravity(const glm::vec3 &gravity);
    void setParticleTexture(glcore::TextureObject texture);

    // @param dt - разница во времени с предыдущим вызовом Advance.
	void update(float deltaSeconds) final;

    // @param program - должна быть активирована перед рисованием.
    // @param worldView - задаёт преобразование из локальных координат
    //   системы частиц в систему координат камеры.
	void draw(const RenderContext& ctx) final;

private:
	void bindParticlePositions(const IShaderProgram &program, const glm::mat4 &worldView);
    void updateParticlePositions(const glm::mat4 &worldView);

    std::unique_ptr<ParticleEmitter> m_pEmitter;
    std::vector<Particle> m_particles;
    glm::vec3 m_gravity;

    // Если true, то надо обновить данные на видеокарт
    // См. паттерн "Dirty Flag": http://gameprogrammingpatterns.com/dirty-flag.html
    bool m_isDirty = false;

    // Хранит текстуру отдельной частицы.
    glcore::TextureObject m_texture;

    // Хранит атрибуты вершин статичного спрайта (текстурированного прямоугольника)
    glcore::VBO m_spriteGeometry;

    // Хранит постоянно изменяющиеся позиции частиц
    glcore::VBO m_particlePositions;
};
