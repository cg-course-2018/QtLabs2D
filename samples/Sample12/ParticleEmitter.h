#pragma once

#include <random>
#include <glm/vec3.hpp>
#include "ClampedNormalDistribution.h"

// Класс Particle моделирует частицу, обладающую
//  - местоположением
//  - скоростью
//  - временем жизни
class Particle
{
public:
    Particle(const glm::vec3 &position,
              const glm::vec3 &velocity,
              float lifetime);

    // Обновляет состояние частицы в соответствии с прошедшим временем.
    void advance(float deltaSeconds, const glm::vec3 &acceleration);

    glm::vec3 getPosition()const;
    bool isAlive()const;

private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    float m_lifetime = 0;
};

// Класс CParticleEmitter моделирует источник частиц, создающий частицы,
//  вылетающие из заданного места в заданном направлении.
// Случайным вариациям поддаются:
//  - дистанция от источника частиц, на которой появляется частица
//  - отклонение направления частицы от направления источника
//  - скорость частицы
//  - время жизни частицы
//  - интервал между генерацией двух частиц
class ParticleEmitter
{
public:
    ParticleEmitter();

    // Обновляет состояние частицы в соответствии с прошедшим временем.
    // @param dt - разница во времени с предыдущим вызовом Advance.
    void advance(float dt);
    bool isEmitReady()const;
    Particle Emit();

    // Задаёт центр источника частиц.
    void setPosition(const glm::vec3 &value);

    // Задаёт разброс расстояния от места появления частицы до центра источника.
    void setDistanceRange(float minValue, float maxValue);

    // Задаёт направление вылета частиц.
    void setDirection(const glm::vec3 &value);

    // Задаёт максимальный угол отклонения направления частицы
    //  от основного направления вылета частиц.
    void setMaxDeviationAngle(float value);

    // Задаёт разброс времени жизни частиц.
    void setLifetimeRange(float minValue, float maxValue);

    // Задаёт разброс времени между вылетом двух частиц.
    void setEmitIntervalRange(float minValue, float maxValue);

    // Задаёт разброс скорости частицы.
    void setSpeedRange(float minValue, float maxValue);

private:
    using linear_random_float = std::uniform_real_distribution<float>;
    using normal_random_float = ClampedNormalDistribution;

    glm::vec3 makeRandomDirection();

    float m_elapsedSeconds = 0;
    float m_nextEmitTime = 0;
    glm::vec3 m_position;
    glm::vec3 m_direction = glm::vec3(0, 1, 0);
    linear_random_float m_distanceRange;
    linear_random_float m_deviationAngleRange;
    normal_random_float m_lifetimeRange;
    normal_random_float m_emitIntervalRange;
    normal_random_float m_speedRange;
    std::mt19937 m_random;
};
