#pragma once
#include "IBallPool.h"
#include "Ball.h"
#include <libmath\RectF.h>
#include <vector>

class BallPool : public IBallPool
{
public:
	BallPool(RectF bounds);
	virtual ~BallPool();

	void update(float deltaSec) override;
	std::vector<IBall*> balls() override;

	bool tryAddBall(const Ball& ball);
private:
	void collide(Ball& b1, Ball& b2);
	float getCollisionTime(const Ball& b1, const Ball& b2);
	//void updateRemainingTime(float dt);
	//void removeDeadBalls();
	void processBoundsColiisions();
	void moveAllBalls(float deltaSeconds);
	void processBallsCollisions(float deltaSeconds);
	void processBallPairCollision(Ball & b1, Ball & b2, float deltaSeconds);

	std::vector<Ball> m_balls;
	RectF m_bounds;
};

