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
};

// Интерфейс для получения доступа к событию клавиатуры
class IKeyEvent
{
public:
	virtual ~IKeyEvent() = default;
	virtual Key getKey() const = 0;
};

// Интерфейс выполняет обработку событий. Перегруженный обработчик должен вернуть true, если событие обработано.
class IInputListener
{
public:
	virtual ~IInputListener() = default;

	virtual bool keyPressEvent(IKeyEvent &event) { return false; };
	virtual bool keyReleaseEvent(IKeyEvent &event) { return false; };
};

} // namespace platform
