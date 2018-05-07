#pragma once
#include "ICameraController.h"
#include "IFlyingCamera.h"
#include "LightSource.h"
#include "MeshP3N3T2.h"
#include "ShaderProgram.h"
#include "Texture2DAtlas.h"
#include <libglcore/libglcore.h>
#include <libplatform/IRenderScene.h>
#include <optional>
#include "Tile.h"

#include "ScoreController.h"

class GameScene
	: public platform::IRenderScene
{
public:
	GameScene();
	~GameScene();

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

	// Позволяет протестировать подсистемы игры путём симуляции игры.
	bool m_shouldSimulateGame = false;

	std::unique_ptr<IFlyingCamera> m_camera;
	std::unique_ptr<ICameraController> m_cameraController;

	ShaderProgram m_programPhong;
	std::optional<Texture2DAtlas> m_textureAtlas;
	MeshP3N3T2 m_sphere;
	Transform3D m_sphereTransform;
	LightSource m_sunlight;
	std::unique_ptr<TileMesh> m_tile;
	ScoreController m_scoreController;
};

void ShowGemeOverMessage();
