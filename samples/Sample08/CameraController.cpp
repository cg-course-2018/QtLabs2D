#include "stdafx.h"
#include "CameraController.h"
#include <glm/trigonometric.hpp>

using platform::Key;

namespace
{
// Линейная скорость движения камеры, единиц в секунду.
constexpr float CAMERA_SPEED = 0.3f;

// Угловая скорость вращения камеры, градусов на пиксель смещения мыши.
constexpr float CAMERA_ANGULAR_SPEED = 0.2f;

bool IsCameraControlKey(Key key)
{
	switch (key)
	{
	case Key::LetterW:
	case Key::LetterA:
	case Key::LetterS:
	case Key::LetterD:
	case Key::Left:
	case Key::Right:
	case Key::Up:
	case Key::Down:
		return true;
	default:
		return false;
	}
}
} // namespace

CameraController::CameraController(IFlyingCamera &camera)
	: m_camera(camera)
{
}

void CameraController::update(float deltaSeconds)
{
	const glm::vec3 movementDir = getRequestedMovementDir();
	m_camera.move(CAMERA_SPEED * movementDir);
}

bool CameraController::keyPressEvent(platform::IKeyEvent &event)
{
	if (IsCameraControlKey(event.getKey()))
	{
		m_pressedKeys.insert(event.getKey());
		return true;
	}
	return false;
}

bool CameraController::keyReleaseEvent(platform::IKeyEvent &event)
{
	if (IsCameraControlKey(event.getKey()))
	{
		m_pressedKeys.erase(event.getKey());
		return true;
	}
	return false;
}

bool CameraController::mousePressEvent(platform::IMouseEvent &event)
{
	if (event.isButtonPressed(platform::MouseButton::Left))
	{
		// Запоминаем текущую позицию мыши как последнюю позицию мыши при перетаскивании.
		// Тем самым мы инициируем перетаскивание.
		const auto position = event.getPosition();
		m_lastDragPos.emplace(position.first, position.second);
	}
	return true;
}

bool CameraController::mouseMoveEvent(platform::IMouseEvent &event)
{
	// Проверяем, выполняется ли перетаскивание
	if (m_lastDragPos)
	{
		const auto position = event.getPosition();
		applyDragRotation(glm::vec2{ position.first, position.second });
	}

	return true;
}

bool CameraController::mouseReleaseEvent(platform::IMouseEvent &event)
{
	// Проверяем, выполняется ли перетаскивание
	if (m_lastDragPos)
	{
		const auto position = event.getPosition();
		applyDragRotation(glm::vec2{ position.first, position.second });

		// Сбрасываем последнюю позицию мыши при перетаскивании.
		// Тем самым мы завершаем перетаскивание.
		m_lastDragPos = std::nullopt;
	}

	return true;
}

IFlyingCamera &CameraController::getCamera()
{
	return m_camera;
}

const IFlyingCamera &CameraController::getCamera() const
{
	return m_camera;
}

bool CameraController::isPressed(platform::Key key) const
{
	return (m_pressedKeys.find(key) != m_pressedKeys.end());
}

glm::vec3 CameraController::getRequestedMovementDir() const
{
	glm::vec3 movementDir{ 0 };
	if (isPressed(Key::LetterW))
	{
		movementDir.z = 1;
	}
	else if (isPressed(Key::LetterS))
	{
		movementDir.z = -1;
	}
	if (isPressed(Key::LetterA))
	{
		movementDir.x = 1;
	}
	else if (isPressed(Key::LetterD))
	{
		movementDir.x = -1;
	}
	if (length(movementDir) > 0)
	{
		movementDir = glm::normalize(movementDir);
	}
	return movementDir;
}

void CameraController::applyDragRotation(const glm::vec2 &newDragPos)
{
	const glm::vec2 delta = newDragPos - *m_lastDragPos;
	m_lastDragPos = newDragPos;

	// Вычисляем углы рысканья и тангажа в радианах.
	const float yaw = glm::radians(delta.x * CAMERA_ANGULAR_SPEED);
	const float pitch = glm::radians(-delta.y * CAMERA_ANGULAR_SPEED);

	m_camera.rotate(yaw, pitch);
}
