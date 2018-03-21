#pragma once
#include <libplatform\IGraphicsScene.h>
#include <libballs2d\IBallPool.h>
#include <libmath\RectF.h>

class PoolScene : public platform::IGraphicsScene
{
public:
	PoolScene(std::shared_ptr<IBallPool> pool);
	virtual ~PoolScene();

	void update(float deltaSeconds) override;
	void redraw(QPainter& painter) override;
private:
	RectF m_bounds;
	std::shared_ptr<IBallPool> m_pool;
};

