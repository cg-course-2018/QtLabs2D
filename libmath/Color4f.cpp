#include "stdafx.h"
#include "Color4f.h"

Color4f::Color4f() 
	: r{ 0 }, g{ 0 }, b{ 0 }, a{ 1 }
{
}

Color4f::Color4f(float r, float g, float b, float a)
	: r{ r }, g{ g }, b{ b }, a{ a }
{
}