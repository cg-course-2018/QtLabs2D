#include "stdafx.h"
#include "PoolTableScene.h"

namespace
{
constexpr float BALL_SIZE = 40; // Размер шарика, пикселей.
constexpr float BALL_SPEED_X = 50; // Скорость по оси Ox, пикселей в секунду.
constexpr float BALL_AMPLITUDE_Y = 50; // Максимальное отклонение по оси Oy, пикселей.
constexpr float BALL_PERIOD = 2; // Период синусоиды, секунд.
constexpr float PI = 3.14159265358f; // Приближение числа π.
const Vector2f BALL_START_POSITION = { 0, 150 }; // Период синусоиды, секунд.
} // namespace

PoolTableScene::PoolTableScene(const RectF &bounds)
	: m_bounds(bounds)
	, m_ballSize(BALL_SIZE, BALL_SIZE)
	, m_ballPosition(BALL_START_POSITION)
{
}

void PoolTableScene::update(float deltaSeconds)
{
	m_time += deltaSeconds;
	m_ballPosition = BALL_START_POSITION + Vector2f{ BALL_SPEED_X * m_time, BALL_AMPLITUDE_Y * std::sin(m_time * 2.f * PI / BALL_PERIOD) };
}

void PoolTableScene::redraw(QPainter &painter)
{
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QBrush(QColor(0xFA, 0xFE, 0x78)));
	painter.drawEllipse(QRectF(m_ballPosition.x, m_ballPosition.y, m_ballSize.x, m_ballSize.y));
}
