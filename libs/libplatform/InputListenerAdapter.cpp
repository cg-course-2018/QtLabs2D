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

	Key getKey() const final
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
		case Qt::Key_W:
			return Key::LetterW;
		case Qt::Key_A:
			return Key::LetterA;
		case Qt::Key_S:
			return Key::LetterS;
		case Qt::Key_D:
			return Key::LetterD;
		case Qt::Key_E:
			return Key::LetterE;
		case Qt::Key_Q:
			return Key::LetterQ;
		}
		return Key::Unknown;
	}

private:
	QKeyEvent *m_event = nullptr;
};

class MouseEventAdapter final : public IMouseEvent
{
public:
	MouseEventAdapter(QMouseEvent *event)
		: m_event(event)
	{
	}

	std::pair<double, double> getPosition() const final
	{
		const auto localPos = m_event->localPos();
		return { localPos.x(), localPos.y() };
	}

	bool isButtonPressed(MouseButton button) const final
	{
		Qt::MouseButtons mask = {};
		switch (button)
		{
		case MouseButton::Left:
			mask = Qt::LeftButton;
			break;
		case MouseButton::Right:
			mask = Qt::RightButton;
			break;
		case MouseButton::Middle:
			mask = Qt::MiddleButton;
			break;
		}
		return ((mask & m_event->buttons()) != 0);
	}

private:
	QMouseEvent *m_event = nullptr;
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

bool InputListenerAdapter::mousePressEvent(QMouseEvent *event)
{
	MouseEventAdapter adapter(event);
	return m_listener.mousePressEvent(adapter);
}

bool InputListenerAdapter::mouseReleaseEvent(QMouseEvent *event)
{
	MouseEventAdapter adapter(event);
	return m_listener.mouseReleaseEvent(adapter);
}

bool InputListenerAdapter::mouseMoveEvent(QMouseEvent *event)
{
	MouseEventAdapter adapter(event);
	return m_listener.mouseMoveEvent(adapter);
}

} // namespace platform
