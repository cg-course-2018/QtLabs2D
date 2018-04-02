#include "stdafx.h"
#include "InputListenerAdapter.h"

namespace platform
{

namespace
{
class KeyEventAdapter : public IKeyEvent
{
public:
	KeyEventAdapter(QKeyEvent *event)
		: m_event(event)
	{
	}

	Key getKey() const
	{
		switch (m_event->key())
		{
		case Qt::Key_Left:
			return Key::Left;
		case Qt::Key_Right:
			return Key::Right;
		case Qt::Key_Up:
			return Key::Up;
		case Qt::Key_Down:
			return Key::Down;
		case Qt::Key_Minus:
			return Key::Minus;
		case Qt::Key_Plus:
			return Key::Plus;
		case Qt::Key_Equal:
			return Key::Equal;
		}
		return Key::Unknown;
	}

private:
	QKeyEvent *m_event = nullptr;
};
} // namespace

InputListenerAdapter::InputListenerAdapter(IInputListener &listener)
	: m_listener(listener)
{
}

bool InputListenerAdapter::keyPressEvent(QKeyEvent *event)
{
	KeyEventAdapter adapter(event);
	return m_listener.keyPressEvent(adapter);
}

bool InputListenerAdapter::keyReleaseEvent(QKeyEvent *event)
{
	KeyEventAdapter adapter(event);
	return m_listener.keyReleaseEvent(adapter);
}

} // namespace platform
