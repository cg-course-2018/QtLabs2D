#include "stdafx.h"

#include <QtWidgets/QApplication>
#include <libplatform/libplatform.h>
#include <libballs2d\BallPool.h>
#include "PoolScene.h"
#include "RandomGenerator.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	platform::RenderWindowOptions opts = { 800, 600 };
	platform::RenderWindow window(opts);

	std::shared_ptr<BallPool> pool = std::make_shared<BallPool>(RectF(0, 0, opts.width, opts.height));
	auto scene = std::make_unique<PoolScene>(pool);

	RandomGenerator gen;
	pool->tryAddBall(Ball(20, Vector2f(20, 40), Vector2f(100, 10)));
	pool->tryAddBall(Ball(20, Vector2f(200, 40), Vector2f(-100, 10)));
	pool->tryAddBall(gen.getRandomBall());
	pool->tryAddBall(gen.getRandomBall());
	pool->tryAddBall(gen.getRandomBall());

	window.setScene(std::move(scene));
	window.show();

	return app.exec();
}
