#pragma once
#include "ICameraController.h"
#include "IFlyingCamera.h"
#include "LightSource.h"
#include "MeshP3N3.h"
#include "ShaderProgram.h"
#include <libglcore/libglcore.h>
#include <libplatform/IRenderScene.h>

class LightingScene
	: public platform::IRenderScene
{
public:
	LightingScene();
	~LightingScene();

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
	void initializeNoLightingProgram();
	void initializeLambertProgram();
	void initializePhongProgram();
	void initializeLights();
	void initializeObjects();
	void setProjectionMatrix(unsigned width, unsigned height);
	void setCameraUniforms();

	std::unique_ptr<IFlyingCamera> m_camera;
	std::unique_ptr<ICameraController> m_cameraController;

	ShaderProgram m_programNoLighting;
	ShaderProgram m_programLambert;
	ShaderProgram m_programPhong;
	ShaderProgram *m_activeProgram = nullptr;
	MeshP3N3 m_cube;
	Transform3D m_cubeTransform;
	MeshP3N3 m_sphere;
	Transform3D m_sphereTransform;
	LightSource m_sunlight;
	MeshP3N3 m_rightLightSphere;
	LightSource m_rightLight;
};
