#pragma once
#include "ICameraController.h"
#include "IFlyingCamera.h"
#include "MeshP3C3.h"
#include <libglcore/libglcore.h>
#include <libplatform/IRenderScene.h>

class Simple3DScene
	: public platform::IRenderScene
{
public:
	Simple3DScene();
	~Simple3DScene();

	void initialize() final;
	void update(float deltaSeconds) final;
	void redraw(unsigned width, unsigned height) final;
	bool keyPressEvent(platform::IKeyEvent &event) final;
	bool keyReleaseEvent(platform::IKeyEvent &event) final;
	bool mousePressEvent(platform::IMouseEvent &event) final;
	bool mouseMoveEvent(platform::IMouseEvent &event) final;
	bool mouseReleaseEvent(platform::IMouseEvent &event) final;

private:
	void initializeShaders();
	void setProjectionMatrix(unsigned width, unsigned height);
	void setViewMatrix();

	std::unique_ptr<IFlyingCamera> m_camera;
	std::unique_ptr<ICameraController> m_cameraController;

	glcore::ProgramObject m_program;
	MeshP3C3 m_cube;
	Transform3D m_cubeTransform;
};
