#pragma once
#include "IInputListener.h"
#include <QtCore/QEvent>

namespace platform
{

class InputListenerAdapter
{
public:
	explicit InputListenerAdapter(IInputListener &listener);

	bool keyPressEvent(QKeyEvent *event);
	bool keyReleaseEvent(QKeyEvent *event);

private:
	IInputListener &m_listener;
};

} // namespace platform
