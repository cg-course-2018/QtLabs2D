#pragma once
#include "IInputListener.h"

#include <QtCore/QRect>

namespace platform
{

class IRenderScene : public IInputListener
{
public:
	virtual ~IRenderScene() = default;

	virtual void initialize(){};
	virtual void update(float deltaSeconds) = 0;
	virtual void redraw(unsigned width, unsigned height) = 0;
};

} // namespace platform
