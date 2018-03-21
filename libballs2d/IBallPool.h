#pragma once

#include <vector>
#include <libballs2d\IBall.h>

class IBallPool
{
public:
	virtual ~IBallPool() = default;

	virtual void update(float deltaSec) = 0;
	virtual std::vector<IBall*> balls() = 0;
};

