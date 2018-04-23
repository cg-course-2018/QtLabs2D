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
	// ���������� true, ���� �������� ������� ���������� ������� � ������� ������.
	bool isPressed(platform::Key key) const;

	// ���������� ������� ����������� �������� ������ � ����������� �� ��������� ����������.
	glm::vec3 getRequestedMovementDir() const;

	// ��������� �������, ��������������� ������ ��� �������������� ����.
	void applyDragRotation(const glm::vec2 &newDragPos);

	IFlyingCamera &m_camera;
	std::unordered_set<platform::Key> m_pressedKeys;
	std::optional<glm::vec2> m_lastDragPos;
};
