#include "stdafx.h"
#include "BallPool.h"

BallPool::BallPool(RectF bounds) : m_bounds(bounds)
{
}

BallPool::~BallPool()
{
}

void BallPool::update(float deltaSec)
{
	//updateRemainingTime(deltaSec);
	//removeDeadBalls();
	moveAllBalls(deltaSec);
	processBoundsColiisions();
	processBallsCollisions(deltaSec);
}

bool BallPool::tryAddBall(const Ball &ball)
{
	/*for (const auto& b : m_balls) {
		if (b.bbox().intersects(ball.bbox()))
			return false;
	}*/
	m_balls.push_back(ball);
	return true;
}

void BallPool::processBoundsColiisions()
{
	for (auto &ball : m_balls) {
		if (ball.bbox().rightEdge() > m_bounds.rightEdge())
		{
			ball.setCenter(Vector2f(m_bounds.rightEdge() - ball.radius(), ball.center().y));
			ball.setSpeed(Vector2f(-ball.speed().x, ball.speed().y));
		}
		else if (ball.bbox().leftEdge() < m_bounds.leftEdge())
		{
			ball.setCenter(Vector2f(m_bounds.leftEdge() + ball.radius(), ball.center().y));
			ball.setSpeed(Vector2f(-ball.speed().x, ball.speed().y));
		}

		if (ball.bbox().topEdge() < m_bounds.topEdge())
		{
			ball.setCenter(Vector2f(ball.center().x, m_bounds.topEdge() + ball.radius()));
			ball.setSpeed(Vector2f(ball.speed().x, -ball.speed().y));
		}
		else
			if (ball.bbox().bottomEdge() > m_bounds.bottomEdge())
			{
				ball.setCenter(Vector2f(ball.center().x, m_bounds.bottomEdge() - ball.radius()));
				ball.setSpeed(Vector2f(ball.speed().x, -ball.speed().y));
			}
	}
}

void BallPool::moveAllBalls(float deltaSeconds)
{
	for (auto &ball : m_balls) {
		ball.move(deltaSeconds);
	}
}

void BallPool::processBallsCollisions(float deltaSeconds)
{
	for (size_t fi = 0; fi < m_balls.size(); ++fi)
	{
		for (size_t si = fi + 1; si < m_balls.size(); ++si)
		{
			Ball& b1 = m_balls[fi];
			Ball& b2 = m_balls[si];
			// проверяем столкновение fi, si
			processBallPairCollision(b1, b2, deltaSeconds);
		}
	}
}

void BallPool::processBallPairCollision(Ball & b1, Ball & b2, float deltaSeconds)
{
	if (dist(b1.center(), b2.center()) <= b1.radius() + b2.radius())
	{
		//Vector2f impulse_before = b1.impulse() + b2.impulse();
		//float energy_before = b1.energy() + b2.energy();

		// возвращаем шары на исходную
		b1.move(-deltaSeconds);
		b2.move(-deltaSeconds);
		// вычисляем точное время до столкновения
		float tc = getCollisionTime(b1, b2);
		b1.move(tc);
		b2.move(tc);
		collide(b1, b2);
		// используем остаток времени после столкновения
		b1.move(deltaSeconds - tc);
		b2.move(deltaSeconds - tc);

		//Vector2f impulse_after = b1.impulse() + b2.impulse();
		//float energy_after = b1.energy() + b2.energy();

		//Q_ASSERT(areFuzzyEqual(impulse_before.x, impulse_after.x));
		//Q_ASSERT(areFuzzyEqual(impulse_before.y, impulse_after.y));
		//Q_ASSERT(areFuzzyEqual(energy_before, energy_after));
	}
}

void BallPool::collide(Ball & b1, Ball & b2)
{
	Vector2f dist_vec = b1.center() - b2.center();
	Vector2f dv = b1.speed() - b2.speed();
	float dist_squared = std::pow(dist_vec.length(), 2);
	float factor = dot(dv, dist_vec) / dist_squared;

	float mass_f1 = 2 * b2.mass() / (b1.mass() + b2.mass());
	float mass_f2 = 2 * b1.mass() / (b1.mass() + b2.mass());

	Vector2f w1 = b1.speed() - (mass_f1 * factor * dist_vec);
	Vector2f w2 = b2.speed() - (mass_f2 * factor * -dist_vec);

	b1.setSpeed(w1);
	b2.setSpeed(w2);
}

float BallPool::getCollisionTime(const Ball &b1, const Ball &b2)
{
	//     v1        v2
	//  ( )-->    ( )->
	//   ----------> cc
	Vector2f cc = b2.center() - b1.center();
	float v1 = b1.speed().projectOn(cc);
	float v2 = b2.speed().projectOn(cc);
	float appr_speed = v1 - v2;
	float dist_now = cc.length();
	float ds = dist_now - (b1.radius() + b2.radius());
	return ds / appr_speed;
}

std::vector<IBall*> BallPool::balls()
{
	std::vector<IBall*> ret;
	auto end = m_balls.end();
	for (auto i = m_balls.begin(); i != end; ++i) 
	{
		IBall* ptr = &(*i);
		ret.push_back(ptr);
	}
	return ret;
}