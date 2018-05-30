#pragma once
#include "ICameraController.h"
#include "IFlyingCamera.h"
#include "LightSource.h"
#include "ShaderProgram.h"
#include "SceneGraph.h"
#include "IsoSurface.h"
#include <libglcore/libglcore.h>
#include <libplatform/IRenderScene.h>
#include <optional>
#include <memory>

class IsosurfaceScene
	: public platform::IRenderScene
{
public:
	IsosurfaceScene();
	~IsosurfaceScene();

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
	void initializePhongProgram();
	void initializeLights();
	void initializeObjects();
	void setProjectionMatrix(unsigned width, unsigned height);
    void setCameraUniforms();

	std::unique_ptr<IFlyingCamera> m_camera;
	std::unique_ptr<ICameraController> m_cameraController;

	IsoSurface m_surface;
	SceneGraphNodePtr m_surfaceMesh;

	glcore::VAO m_vao;
	ShaderProgram m_phongProgram;
	LightSource m_sunlight;
};

void ShowGemeOverMessage();
