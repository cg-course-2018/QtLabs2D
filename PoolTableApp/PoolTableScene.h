#pragma once
#include <QtGui/QPainter>
#include <libmath\RectF.h>
#include <libplatform/IGraphicsScene.h>

class PoolTableScene : public platform::IGraphicsScene
{
public:
	PoolTableScene(const Rect2F &bounds);

	void update(float deltaSeconds) final;
	void redraw(QPainter &painter) final;

private:
	Vector2f m_ballPosition;
	const Vector2f m_ballSize;
	const Rect2F m_bounds;
	float m_time = 0;
};
