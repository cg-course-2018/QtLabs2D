#pragma once

#include <QtCore/QRect>

namespace platform
{

class IRenderScene
{
public:
	virtual ~IRenderScene() = default;

	virtual void initialize() {};
	virtual void update(float deltaSeconds) = 0;
	virtual void redraw(unsigned width, unsigned height) = 0;
};

}
