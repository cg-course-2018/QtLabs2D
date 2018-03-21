#pragma once
#include "IBall.h"
#include <libmath/Color4f.h>

class Ball : public IBall
{
public:
	Ball(float r, Vector2f center, Vector2f speed = Vector2f{}, Color4f color = Color4f(0xFA/255.f, 0xFE/255.f, 0x78/255.f));
	~Ball();
	RectF bbox() override;
	Color4f color() override;

	float mass() const;
	float energy() const;
	Vector2f impulse() const;
	
	void setRadius(float r) { m_radius = r; }
	float radius() const { return m_radius; }

	void setCenter(const Vector2f& pos) { m_center = pos; }
	Vector2f center() const { return m_center; }

	void setSpeed(const Vector2f& spd) { m_speed = spd; }
	Vector2f speed() const { return m_speed; }

	void move(float dt);


private:
	float m_radius;
	Vector2f m_center;
	Vector2f m_speed;
	float m_remainingTime;
	Color4f m_color;
};



