#pragma once
#include <vector>

class IBall;

class IPoolTable 
{
public:
	
	IPoolTable();
	virtual ~IPoolTable() = default;

	virtual std::vector<IBall*> balls() = 0;
	virtual void update(float deltaSec) = 0;
};

