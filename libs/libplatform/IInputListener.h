#pragma once

namespace platform
{

// Коды нажатых клавиш.
enum class Key
{
	Unknown,
	Equal,
	Plus,
	Minus,
	Left,
	Right,
	Up,
	Down,
	LetterW,
	LetterA,
	LetterS,
	LetterD,
	LetterE,
	LetterQ,
};

enum class MouseButton
{
	Left,
	Right,
	Middle,
};

// Интерфейс для получения доступа к событию клавиатуры
class IKeyEvent
{
public:
	virtual ~IKeyEvent() = default;

	// Возвращает код нажатой клавиши.
	virtual Key getKey() const = 0;
};

class IMouseEvent
{
public:
	virtual ~IMouseEvent() = default;

	virtual std::pair<double, double> getPosition() const = 0;
	virtual bool isButtonPressed(MouseButton button) const = 0;
};

// Интерфейс выполняет обработку событий. Перегруженный обработчик должен вернуть true, если событие обработано.
class IInputListener
{
public:
	virtual ~IInputListener() = default;

	virtual bool keyPressEvent(IKeyEvent &event) { return false; };
	virtual bool keyReleaseEvent(IKeyEvent &event) { return false; };
	virtual bool mousePressEvent(IMouseEvent &event) { return false; };
	virtual bool mouseReleaseEvent(IMouseEvent &event) { return false; };
	virtual bool mouseMoveEvent(IMouseEvent &event) { return false; };
};

} // namespace platform
