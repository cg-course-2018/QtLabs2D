#pragma once
#include "ICameraController.h"
#include "IFlyingCamera.h"
#include "LightSource.h"
#include "ShaderProgram.h"
#include "SceneGraph.h"
#include <libglcore/libglcore.h>
#include <libplatform/IRenderScene.h>
#include <optional>
#include <memory>

class NurbsScene
	: public platform::IRenderScene
{
public:
	NurbsScene();
	~NurbsScene();

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

	glcore::VAO m_vao;
	SceneGraphNodePtr m_teapotNode;
    ShaderProgram m_particlesProgram;
	LightSource m_sunlight;
};

void ShowGemeOverMessage();
