#pragma once

#include <QtGui/QPainter>

namespace platform
{

class IGraphicsScene
{
public:
	virtual ~IGraphicsScene() = default;

	virtual void update(float deltaSeconds) = 0;
	virtual void redraw(QPainter& painter) = 0;
};

}
