#include "stdafx.h"
#include "PoolTable.h"
#include "libmath/Vector2f.h"


PoolTable::PoolTable()
{
}


PoolTable::~PoolTable()
{
}

std::vector<IBall*> PoolTable::balls()
{
	std::vector<IBall*> result;
	for (int i = 0; i < m_balls.size(); i++)//заполнение массива result указателями на элементы m_balls
	{
		result.push_back(m_balls[i].get());
	}
	return result;
}

void PoolTable::updateRemainingTime(float dt)
{
	for (auto& b : m_balls) {
		//b.setRemainingTime(b.remainingTime() - dt);
	}
}

void PoolTable::update(float deltaSec)
{
	updateRemainingTime(deltaSec);

	//removeDeadBalls();

	for (auto &ball : m_balls) {
		//ball->move(deltaSec);
		if (ball->bbox().rightEdge() > m_bounds.rightEdge())
		{
			ball->setCenter(Vector2f(m_bounds.right() - ball->radius(), ball->center().y));
			ball->setSpeed(Vector2f(-ball->speed().x, ball->speed().y));
		}
		else if (ball->bbox().leftEdge() < m_bounds.left())
		{
			ball->setCenter(Vector2f(m_bounds.left() + ball->radius(), ball->center().y));
			ball->setSpeed(Vector2f(-ball->speed().x, ball->speed().y));
		}

		if (ball->bbox().topEdge() < m_bounds.top())
		{
			ball->setCenter(Vector2f(ball->center().x, m_bounds.top() + ball->radius()));
			ball->setSpeed(Vector2f(ball->speed().x, -ball->speed().y));
		}
		else
			if (ball->bbox().bottomEdge() > m_bounds.bottom())
			{
				ball->setCenter(Vector2f(ball->center().x, m_bounds.bottom() - ball->radius()));
				ball->setSpeed(Vector2f(ball->speed().x, -ball->speed().y));
			}
	}

	for (size_t fi = 0; fi < m_balls.size(); ++fi)
	{
		for (size_t si = fi + 1; si < m_balls.size(); ++si)
		{
			Ball& b1 = m_balls[fi];
			Ball& b2 = m_balls[si];
			// проверяем столкновение fi, si
			if (dist(b1.center(), b2.center()) <= b1.radius() + b2.radius())
			{
				Vector2f impulse_before = b1.impulse() + b2.impulse();
				float energy_before = b1.energy() + b2.energy();

				// возвращаем шары на исходную
				b1.move(-deltaSec);
				b2.move(-deltaSec);
				// вычисляем точное время до столкновения
				float tc = getCollisionTime(b1, b2);
				b1.move(tc);
				b2.move(tc);
				collide(b1, b2);
				// используем остаток времени после столкновения
				b1.move(deltaSec - tc);
				b2.move(deltaSec - tc);

				Vector2f impulse_after = b1.impulse() + b2.impulse();
				float energy_after = b1.energy() + b2.energy();

				Q_ASSERT(areFuzzyEqual(impulse_before.x, impulse_after.x));
				Q_ASSERT(areFuzzyEqual(impulse_before.y, impulse_after.y));
				Q_ASSERT(areFuzzyEqual(energy_before, energy_after));
			}
		}
	}
}
