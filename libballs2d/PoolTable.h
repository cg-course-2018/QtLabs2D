#pragma once
#include "IPoolTable.h"
#include <memory>
#include "Ball.h"

class PoolTable : public IPoolTable
{
public:
	PoolTable();
	~PoolTable();

	std::vector<IBall*> balls() override;
	void updateRemainingTime(float dt);

	void update(float deltaSec) override;
	void updateRemainingTime();

private :
	std::vector<std::unique_ptr<Ball>> m_balls;
	void updateRemainingTime();
};

