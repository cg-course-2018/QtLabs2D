#include "stdafx.h"
#include "Simple3DScene.h"
#include "CameraController.h"
#include "FlyingCamera.h"
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
const glm::vec3 CAMERA_POSITION = { 0, 4, 10 };
const glm::vec3 CAMERA_TARGET = { 0, 0, 0 };
const glm::vec3 CAMERA_UP = { 0, 1, 0 };

// Базовый примитив куба имеет размер 2, поэтому масштаб задаём равным желаемому размеру, делённому на 2
constexpr float CUBE_SIZE = 5.f;
constexpr float CUBE_SCALE = CUBE_SIZE / 2.f;

// Скорость вращения куба, градусов в секунду.
constexpr float CUBE_ROTATE_SPEED = 20;
} // namespace

Simple3DScene::Simple3DScene()
	: m_camera(std::make_unique<FlyingCamera>(CAMERA_POSITION, CAMERA_TARGET, CAMERA_UP))
	, m_cameraController(std::make_unique<CameraController>(*m_camera))
{
}

Simple3DScene::~Simple3DScene() = default;

void Simple3DScene::initialize()
{
	glcore::initGLBinding();
	initializeShaders();

	// Включаем тест глубины.
	glEnable(GL_DEPTH_TEST);

	// Включаем отсечение задних граней
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	const MeshDataP3C3 data = tesselateCube();
	m_cube.init(data);
	m_cube.bindAttributes(m_program, "i_position", "i_color");

	m_cubeTransform.scaleBy(CUBE_SCALE);
	m_cube.setTransform(m_cubeTransform);
}

void Simple3DScene::update(float deltaSeconds)
{
	m_cameraController->update(deltaSeconds);

	// Вращаем куб вокруг оси Oy (вертикальной оси).
	const float cubeRotation = glm::radians(CUBE_ROTATE_SPEED * deltaSeconds);
	m_cubeTransform.rotateBy(glm::angleAxis(cubeRotation, glm::vec3{ 0, 1, 0 }));
	m_cube.setTransform(m_cubeTransform);
}

void Simple3DScene::redraw(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
	glUseProgram(m_program);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Устанавливаем матрицу ортографического проецирования.
	setProjectionMatrix(width, height);

	// Устанавливаем матрицу перехода в координаты наблюдателя.
	setViewMatrix();

	m_cube.updateUniforms(m_program, "u_world_matrix");
	m_cube.draw();
}

bool Simple3DScene::keyPressEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyPressEvent(event);
}

bool Simple3DScene::keyReleaseEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyReleaseEvent(event);
}

bool Simple3DScene::mousePressEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mousePressEvent(event);
}

bool Simple3DScene::mouseMoveEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseMoveEvent(event);
}

bool Simple3DScene::mouseReleaseEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseReleaseEvent(event);
}

void Simple3DScene::initializeShaders()
{
	platform::ResourceLoader loader;

	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::compileShader(GL_VERTEX_SHADER, loader.loadAsString("draw3d.vert")));
	shaders.emplace_back(glcore::compileShader(GL_FRAGMENT_SHADER, loader.loadAsString("draw3d.frag")));
	m_program = glcore::linkProgram(shaders);
}

void Simple3DScene::setProjectionMatrix(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);

	// Вычисляем матрицу перспективного проецирования.
	// Затем передаём матрицу как константу в графической программе.
	const float fieldOfView = glm::radians(70.f);
	const float aspect = float(width) / float(height);
	const float zNear = 0.05f;
	const float zFar = 50.f;
	const glm::mat4 mat = glm::perspective(fieldOfView, aspect, zNear, zFar);

	glUniformMatrix4fv(glGetUniformLocation(m_program, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(mat));
}

void Simple3DScene::setViewMatrix()
{
	// Матрица перехода в координаты наблюдателя определяется состоянием камеры.
	// Мы передаём матрицу как константу в графической программе.
	const glm::mat4 mat = m_camera->getViewTransform();
	glUniformMatrix4fv(glGetUniformLocation(m_program, "u_view_matrix"), 1, GL_FALSE, glm::value_ptr(mat));
}
