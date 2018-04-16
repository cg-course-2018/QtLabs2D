#pragma once
#include "ICameraController.h"
#include "IFlyingCamera.h"
#include <optional>
#include <unordered_set>

class CameraController final : public ICameraController
{
public:
	explicit CameraController(IFlyingCamera &camera);

	void update(float deltaSeconds) final;
	bool keyPressEvent(platform::IKeyEvent &event) final;
	bool keyReleaseEvent(platform::IKeyEvent &event) final;
	bool mousePressEvent(platform::IMouseEvent &event) final;
	bool mouseMoveEvent(platform::IMouseEvent &event) final;
	bool mouseReleaseEvent(platform::IMouseEvent &event) final;

	IFlyingCamera &getCamera();
	const IFlyingCamera &getCamera() const;

private:
	// Возвращает true, если заданная клавиша оставалась нажатой к моменту вызова.
	bool isPressed(platform::Key key) const;

	// Возвращает текущее направление движения камеры в зависимости от состояния клавиатуры.
	glm::vec3 getRequestedMovementDir() const;

	// Применяет поворот, соответствующий сдвигу при перетаскивании мыши.
	void applyDragRotation(const glm::vec2 &newDragPos);

	IFlyingCamera &m_camera;
	std::unordered_set<platform::Key> m_pressedKeys;
	std::optional<glm::vec2> m_lastDragPos;
};
