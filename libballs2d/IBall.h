#pragma once

#include <libmath/RectF.h>
#include <libmath/Color4f.h>


class IBall
{
public:
	virtual ~IBall() = default;
	virtual RectF bbox() = 0;
	virtual Color4f color() = 0;
};

