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

PoolTableScene::PoolTableScene(const Rect2F &bounds)
{
}

void PoolTableScene::update(float deltaSeconds)
{
}

void PoolTableScene::redraw(QPainter &painter)
{
}
