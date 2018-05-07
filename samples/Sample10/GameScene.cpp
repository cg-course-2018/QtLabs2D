#include "stdafx.h"
#include "GameScene.h"
#include "CameraController.h"
#include "FlyingCamera.h"
#include "TextureUtils.h"
#include <algorithm>
#include <glbinding/gl32core/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <libplatform/ResourceLoader.h>

// Используем функции из gl32core, экспортированные библиотекой glbinding.
using namespace gl32core;

namespace
{

// Три вектора, описывающих первоначальное положение камеры:
//  - положение камеры
//  - положение цели, на которую направлена камера
//  - направление "вверх" для камеры
const glm::vec3 CAMERA_POSITION = { 0, 5, 12 };
const glm::vec3 CAMERA_TARGET = { 0, 0, 0 };
const glm::vec3 CAMERA_UP = { 0, 1, 0 };

// Базовый примитив куба имеет размер 2, поэтому масштаб задаём равным желаемому размеру, делённому на 2
// Аналогично для сферы.
constexpr float SPHERE_SIZE = 7.f;
constexpr float SPHERE_SCALE = SPHERE_SIZE / 2.f;
const glm::vec3 SPHERE_POSITION = glm::vec3{ 5, 0, 0 };

// Скорость вращения куба, градусов в секунду.
constexpr float CUBE_ROTATE_SPEED = 20;
} // namespace

GameScene::GameScene()
	: m_camera(std::make_unique<FlyingCamera>(CAMERA_POSITION, CAMERA_TARGET, CAMERA_UP))
	, m_cameraController(std::make_unique<CameraController>(*m_camera))
{
}

GameScene::~GameScene() = default;

void GameScene::initialize()
{
	glcore::initGLBinding();
	initializeShaders();
	initializeLights();
	initializeObjects();

	// Включаем тест глубины.
	glEnable(GL_DEPTH_TEST);

	// Включаем отсечение задних граней
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void GameScene::update(float deltaSeconds)
{
	m_cameraController->update(deltaSeconds);

	// Вращаем куб вокруг оси Oy (вертикальной оси).
	const float cubeRotation = glm::radians(CUBE_ROTATE_SPEED * deltaSeconds);
	m_sphereTransform.rotateBy(glm::angleAxis(cubeRotation, glm::vec3{ 0, 1, 0 }));
	m_sphere.setTransform(m_sphereTransform);

	m_tile->update(deltaSeconds);
}

void GameScene::redraw(unsigned width, unsigned height)
{
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	m_programPhong.bind();
	utils::setLightSource0(m_programPhong, m_sunlight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Устанавливаем матрицу ортографического проецирования.
	setProjectionMatrix(width, height);

	// Устанавливаем uniform-переменные шейдера в соответствие с состоянием камеры .
	setCameraUniforms();

	m_sphere.updateUniforms(m_programPhong);
	m_sphere.draw();

	m_tile->draw(m_programPhong);
}

bool GameScene::keyPressEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyPressEvent(event);
}

bool GameScene::keyReleaseEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyReleaseEvent(event);
}

bool GameScene::mousePressEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mousePressEvent(event);
}

bool GameScene::mouseMoveEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseMoveEvent(event);
}

bool GameScene::mouseReleaseEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseReleaseEvent(event);
}

void GameScene::initializeShaders()
{
	initializePhongProgram();
}

void GameScene::initializePhongProgram()
{
	platform::ResourceLoader loader;
	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::compileShader(GL_VERTEX_SHADER, loader.loadAsString("res10/phong_lighting.vert")));
	shaders.emplace_back(glcore::compileShader(GL_FRAGMENT_SHADER, loader.loadAsString("res10/phong_lighting.frag")));
	auto program = glcore::linkProgram(shaders);

	std::vector<AttributeInfo> attributes = {
		{ AttributePosition, "i_position" },
		{ AttributeNormal, "i_normal" },
		{ AttributeTexCoord, "i_texture_uv" },
	};
	std::vector<UniformInfo> uniforms = {
		{ UniformWorldMatrix, "u_world_matrix" },
		{ UniformViewMatrix, "u_view_matrix" },
		{ UniformViewerPosition, "u_viewer_position" },
		{ UniformProjectionMatrix, "u_projection_matrix" },
		{ UniformNormalWorldMatrix, "u_normal_world_matrix" },
		{ UniformLight0Position, "u_light0.position" },
		{ UniformLight0Diffuse, "u_light0.diffuse" },
		{ UniformLight0Specular, "u_light0.specular" },
		{ UniformLight1Position, "u_light1.position" },
		{ UniformLight1Diffuse, "u_light1.diffuse" },
		{ UniformLight1Specular, "u_light1.specular" },
		{ UniformColorMap, "u_color_map" },
		{ UniformColorMapRect, "u_color_map_rect" },
	};

	m_programPhong.init(std::move(program), uniforms, attributes);
}

void GameScene::initializeLights()
{
	glm::vec4 sunDiffuse{ 1, 1, 1, 1 };
	glm::vec4 sunSpecular{ 1, 1, 1, 1 };
	glm::vec3 sunDirection{ 0, 1, 0 };
	m_sunlight = utils::makeDirectedLightSource(sunDirection, sunDiffuse, sunSpecular);
}

void GameScene::initializeObjects()
{
	m_textureAtlas.emplace("res10/spites.plist");

	{
		auto sphereMaterial = std::make_shared<Material>();
		sphereMaterial->colorMapId = m_textureAtlas->getTextureId();
		sphereMaterial->colorMapRect = m_textureAtlas->getFrameRect("mushroomRed.png");

		const MeshDataP3N3T2 data = tesselateSphere(sphereMaterial, 25, 25);
		m_sphere.init(data);
		m_sphere.bindAttributes(m_programPhong);
		m_sphereTransform.scaleBy(SPHERE_SCALE);
		m_sphereTransform.moveBy(SPHERE_POSITION);
		m_sphere.setTransform(m_sphereTransform);
		
		m_tile = std::make_unique<TileMesh>(m_textureAtlas.value(), "mushroomRed.png", "flyFly2.png");
		m_tile->initialize(m_programPhong);
	}
}

void GameScene::setProjectionMatrix(unsigned width, unsigned height)
{
	// Вычисляем матрицу перспективного проецирования.
	// Затем передаём матрицу как константу в графической программе.
	const float fieldOfView = glm::radians(70.f);
	const float aspect = float(width) / float(height);
	const float zNear = 0.05f;
	const float zFar = 50.f;
	const glm::mat4 mat = glm::perspective(fieldOfView, aspect, zNear, zFar);

	if (int location = m_programPhong.getUniform(UniformProjectionMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void GameScene::setCameraUniforms()
{
	const glm::mat4 mat = m_camera->getViewTransform();
	if (int location = m_programPhong.getUniform(UniformViewMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	const glm::vec3 pos = m_camera->getViewPosition();
	if (int location = m_programPhong.getUniform(UniformViewerPosition); location != -1)
	{
		glUniform3fv(location, 1, glm::value_ptr(pos));
	}
}
