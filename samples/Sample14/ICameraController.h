#pragma once
#include <libplatform/IInputListener.h>

class ICameraController : public platform::IInputListener
{
public:
	virtual ~ICameraController() = default;

	virtual void update(float deltaSeconds) = 0;
};
