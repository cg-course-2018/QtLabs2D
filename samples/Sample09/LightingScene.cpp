#include "stdafx.h"
#include "LightingScene.h"
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
const glm::vec3 CAMERA_POSITION = { 0, 5, 12 };
const glm::vec3 CAMERA_TARGET = { 0, 0, 0 };
const glm::vec3 CAMERA_UP = { 0, 1, 0 };

// Базовый примитив куба имеет размер 2, поэтому масштаб задаём равным желаемому размеру, делённому на 2
// Аналогично для сферы.
constexpr float CUBE_SIZE = 4.5f;
constexpr float CUBE_SCALE = CUBE_SIZE / 2.f;
const glm::vec3 CUBE_POSITION = glm::vec3{ -5, 0, 0 };
constexpr float SPHERE_SIZE = 7.f;
constexpr float SPHERE_SCALE = SPHERE_SIZE / 2.f;
const glm::vec3 SPHERE_POSITION = glm::vec3{ 1, 0, -3 };

// Скорость вращения куба, градусов в секунду.
constexpr float CUBE_ROTATE_SPEED = 20;
} // namespace

LightingScene::LightingScene()
	: m_camera(std::make_unique<FlyingCamera>(CAMERA_POSITION, CAMERA_TARGET, CAMERA_UP))
	, m_cameraController(std::make_unique<CameraController>(*m_camera))
{
}

LightingScene::~LightingScene() = default;

void LightingScene::initialize()
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

void LightingScene::update(float deltaSeconds)
{
	m_cameraController->update(deltaSeconds);

	// Вращаем куб вокруг оси Oy (вертикальной оси).
	const float cubeRotation = glm::radians(CUBE_ROTATE_SPEED * deltaSeconds);
	m_cubeTransform.rotateBy(glm::angleAxis(cubeRotation, glm::vec3{ 0, 1, 0 }));
	m_cube.setTransform(m_cubeTransform);
}

void LightingScene::redraw(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
	m_activeProgram->bind();
	utils::setLightSource0(*m_activeProgram, m_sunlight);

	// TODO: включите снова источник света №1
	// utils::setLightSource1(*m_activeProgram, m_rightLight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Устанавливаем матрицу ортографического проецирования.
	setProjectionMatrix(width, height);

	// Устанавливаем матрицу перехода в координаты наблюдателя.
	setViewMatrix();

	m_cube.updateUniforms(*m_activeProgram);
	m_cube.draw();

	// TODO: раскомментируйте рисование серой сферы.
	// m_sphere.updateUniforms(*m_activeProgram);
	// m_sphere.draw();

	// TODO: раскомментируйте рисование визуализации источника света.
	// m_rightLightSphere.updateUniforms(*m_activeProgram);
	// m_rightLightSphere.draw();
}

bool LightingScene::keyPressEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyPressEvent(event);
}

bool LightingScene::keyReleaseEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyReleaseEvent(event);
}

bool LightingScene::mousePressEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mousePressEvent(event);
}

bool LightingScene::mouseMoveEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseMoveEvent(event);
}

bool LightingScene::mouseReleaseEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseReleaseEvent(event);
}

void LightingScene::initializeShaders()
{
	initializeNoLightingProgram();
	initializeLambertProgram();
	initializePhongProgram();

	// Выбираем активную программу.
	m_activeProgram = &m_programNoLighting;
	// TODO: включите m_programLambert вместо m_programNoLighting
}

void LightingScene::initializeNoLightingProgram()
{
	platform::ResourceLoader loader;
	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::compileShader(GL_VERTEX_SHADER, loader.loadAsString("res09/no_lighting.vert")));
	shaders.emplace_back(glcore::compileShader(GL_FRAGMENT_SHADER, loader.loadAsString("res09/no_lighting.frag")));
	auto program = glcore::linkProgram(shaders);

	std::vector<AttributeInfo> attributes = {
		{ AttributePosition, "i_position" },
	};
	std::vector<UniformInfo> uniforms = {
		{ UniformWorldMatrix, "u_world_matrix" },
		{ UniformViewMatrix, "u_view_matrix" },
		{ UniformProjectionMatrix, "u_projection_matrix" },
	};

	m_programNoLighting.init(std::move(program), uniforms, attributes);
}

void LightingScene::initializeLambertProgram()
{
	platform::ResourceLoader loader;
	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::compileShader(GL_VERTEX_SHADER, loader.loadAsString("res09/lambert_lighting.vert")));
	shaders.emplace_back(glcore::compileShader(GL_FRAGMENT_SHADER, loader.loadAsString("res09/lambert_lighting.frag")));
	auto program = glcore::linkProgram(shaders);

	std::vector<AttributeInfo> attributes = {
		{ AttributePosition, "i_position" },
		{ AttributeNormal, "i_normal" },
	};
	std::vector<UniformInfo> uniforms = {
		{ UniformWorldMatrix, "u_world_matrix" },
		{ UniformViewMatrix, "u_view_matrix" },
		{ UniformProjectionMatrix, "u_projection_matrix" },
		{ UniformNormalWorldMatrix, "u_normal_world_matrix" },
		{ UniformLight0Position, "u_light0.position" },
		{ UniformLight0Diffuse, "u_light0.diffuse" },
		{ UniformLight1Position, "u_light1.position" },
		{ UniformLight1Diffuse, "u_light1.diffuse" },
		{ UniformMaterialDiffuse, "u_material.diffuse" },
		{ UniformMaterialEmission, "u_material.emission" },
	};

	m_programLambert.init(std::move(program), uniforms, attributes);
}

void LightingScene::initializePhongProgram()
{
	// TODO: проинициализируйте программу Phong подобно программе Lambert,
	//  указав правильные атрибуты.
}

void LightingScene::initializeLights()
{
	glm::vec4 sunDiffuse{ 1, 1, 1, 1 };
	glm::vec4 sunSpecular{ 1, 1, 1, 1 };
	glm::vec3 sunDirection{ 0, 1, 0 };
	m_sunlight = utils::makeDirectedLightSource(sunDirection, sunDiffuse, sunSpecular);

	glm::vec4 rightDiffuse{ 1, 0, 0, 1 };
	glm::vec4 rightSpecular{ 1, 0, 0, 1 };
	glm::vec3 rightPosition{ 8, 0, 5 };
	m_rightLight = utils::makePositionedLightSource(rightPosition, rightDiffuse, rightSpecular);
}

void LightingScene::initializeObjects()
{
	{
		const Material cubeMaterial{
			// TODO: замените значения emission цвета на 0.15, 0.15, 0.15, 1
			glm::vec4(0, 0, 0, 1),
			glm::vec4(0.7, 0.7, 0.7, 1),
			glm::vec4(0.7, 0.7, 0.7, 1),
		};

		const MeshDataP3N3 data = tesselateCube(cubeMaterial);
		m_cube.init(data);
		m_cube.bindAttributes(*m_activeProgram);

		m_cubeTransform.scaleBy(CUBE_SCALE);
		m_cubeTransform.moveBy(CUBE_POSITION);
		m_cube.setTransform(m_cubeTransform);
	}
	{
		const Material sphereMaterial{
			// TODO: замените значения emission цвета на 0.15, 0.15, 0.15, 1
			glm::vec4(0, 0, 0, 1),
			glm::vec4(0.7, 0.7, 0.7, 1),
			glm::vec4(0.7, 0.7, 0.7, 1),
		};

		const MeshDataP3N3 data = tesselateSphere(sphereMaterial, 25, 25);
		m_sphere.init(data);
		m_sphere.bindAttributes(*m_activeProgram);
		m_sphereTransform.scaleBy(SPHERE_SCALE);
		m_sphereTransform.moveBy(SPHERE_POSITION);
		m_sphere.setTransform(m_sphereTransform);
	}
	{
		// Эта сфера представляет визуализацию источника света,
		//  поэтому её положение и цвет определяется источником света
		const Material rightLightMaterial{
			m_rightLight.diffuse,
			glm::vec4(0),
			glm::vec4(0),
		};
		const MeshDataP3N3 data = tesselateSphere(rightLightMaterial, 20, 20);
		m_rightLightSphere.init(data);
		m_rightLightSphere.bindAttributes(*m_activeProgram);

		Transform3D rightLightTransform;
		rightLightTransform.moveBy(glm::vec3(m_rightLight.position));
		m_rightLightSphere.setTransform(rightLightTransform);
	}
}

void LightingScene::setProjectionMatrix(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);

	// Вычисляем матрицу перспективного проецирования.
	// Затем передаём матрицу как константу в графической программе.
	const float fieldOfView = glm::radians(70.f);
	const float aspect = float(width) / float(height);
	const float zNear = 0.05f;
	const float zFar = 50.f;
	const glm::mat4 mat = glm::perspective(fieldOfView, aspect, zNear, zFar);

	if (int location = m_activeProgram->getUniform(UniformProjectionMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void LightingScene::setViewMatrix()
{
	// Матрица перехода в координаты наблюдателя определяется состоянием камеры.
	// Мы передаём матрицу как константу в графической программе.
	const glm::mat4 mat = m_camera->getViewTransform();
	if (int location = m_activeProgram->getUniform(UniformViewMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
}
