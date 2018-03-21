#include "stdafx.h"
#include "PoolScene.h"

PoolScene::PoolScene(std::shared_ptr<IBallPool> pool)
	: IGraphicsScene(), m_pool(pool)
{
}


PoolScene::~PoolScene()
{
}

void PoolScene::update(float deltaSeconds)
{
	m_pool->update(deltaSeconds);
}

void PoolScene::redraw(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing);
	auto balls = m_pool->balls();
	for (const auto& ball : balls)
	{
		auto box = ball->bbox();
		auto c = ball->color();
		painter.setBrush(QBrush(QColor::fromRgb(255 * c.r, 255 * c.g, 255 * c.b, 255 * c.a)));
		painter.drawEllipse(QRectF(box.leftEdge(), box.topEdge(), box.width(), box.height()));
	}
}