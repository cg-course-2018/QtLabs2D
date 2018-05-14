#include "stdafx.h"
#include "ParticlesScene.h"
#include "CameraController.h"
#include "FlyingCamera.h"
#include "ParticlesSceneLoader.h"
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
const glm::vec3 CAMERA_POSITION = { -15, 0, -15 };
const glm::vec3 CAMERA_TARGET = { 0, 0, 0 };
const glm::vec3 CAMERA_UP = { 0, 1, 0 };
} // namespace

ParticlesScene::ParticlesScene()
	: m_camera(std::make_unique<FlyingCamera>(CAMERA_POSITION, CAMERA_TARGET, CAMERA_UP))
	, m_cameraController(std::make_unique<CameraController>(*m_camera))
{
}

ParticlesScene::~ParticlesScene() = default;

void ParticlesScene::initialize()
{
	glcore::initGLBinding();

	// Создаём VAO и навсегда переключаемся на него.
	m_vao = glcore::createVAO();
	glBindVertexArray(m_vao);

	initializeShaders();
	initializeLights();
	initializeObjects();

	// Включаем тест глубины.
	glEnable(GL_DEPTH_TEST);

	// Включаем отсечение задних граней
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	m_rootNode = ParticlesSceneLoader::loadSceneGraph("res12/scene.json");
}

void ParticlesScene::update(float deltaSeconds)
{
	m_cameraController->update(deltaSeconds);
	m_rootNode->update(deltaSeconds);
}

void ParticlesScene::redraw(unsigned width, unsigned height)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	m_particlesProgram.bind();
	utils::setLightSource0(m_particlesProgram, m_sunlight);

	// Устанавливаем матрицу ортографического проецирования.
	setProjectionMatrix(width, height);

	// Устанавливаем uniform-переменные шейдера в соответствие с состоянием камеры .
	setCameraUniforms();

	RenderContext ctx{ m_particlesProgram };
	ctx.viewMat4 = m_camera->getViewTransform();
	m_rootNode->draw(ctx);
}

bool ParticlesScene::keyPressEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyPressEvent(event);
}

bool ParticlesScene::keyReleaseEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyReleaseEvent(event);
}

bool ParticlesScene::mousePressEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mousePressEvent(event);
}

bool ParticlesScene::mouseMoveEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseMoveEvent(event);
}

bool ParticlesScene::mouseReleaseEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseReleaseEvent(event);
}

void ParticlesScene::initializeShaders()
{
	initializePhongProgram();
}

void ParticlesScene::initializePhongProgram()
{
	platform::ResourceLoader loader;
	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::compileShader(GL_VERTEX_SHADER, loader.loadAsString("res12/particles.vert")));
	shaders.emplace_back(glcore::compileShader(GL_FRAGMENT_SHADER, loader.loadAsString("res12/particles.frag")));
	auto program = glcore::linkProgram(shaders);

	std::vector<AttributeInfo> attributes = {
		{ AttributeInstancePosition, "a_particle_pos" },
		{ AttributeTexCoord, "a_texture_uv" },
	};
	std::vector<UniformInfo> uniforms = {
		{ UniformWorldMatrix, "u_world_matrix" },
		{ UniformViewMatrix, "u_view_matrix" },
		{ UniformProjectionMatrix, "u_projection_matrix" },
		{ UniformColorMap, "u_emissive_map" },
	};

	m_particlesProgram.init(std::move(program), uniforms, attributes);
}

void ParticlesScene::initializeLights()
{
	glm::vec4 sunDiffuse{ 1, 1, 1, 1 };
	glm::vec4 sunSpecular{ 1, 1, 1, 1 };
	glm::vec3 sunDirection{ 0, 1, 0 };
	m_sunlight = utils::makeDirectedLightSource(sunDirection, sunDiffuse, sunSpecular);
}

void ParticlesScene::initializeObjects()
{
}

void ParticlesScene::setProjectionMatrix(unsigned width, unsigned height)
{
	// Вычисляем матрицу перспективного проецирования.
	// Затем передаём матрицу как константу в графической программе.
	const float fieldOfView = glm::radians(70.f);
	const float aspect = float(width) / float(height);
	const float zNear = 0.05f;
	const float zFar = 50.f;
	const glm::mat4 mat = glm::perspective(fieldOfView, aspect, zNear, zFar);

	if (int location = m_particlesProgram.getUniform(UniformProjectionMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void ParticlesScene::setCameraUniforms()
{
	const glm::mat4 mat = m_camera->getViewTransform();
	if (int location = m_particlesProgram.getUniform(UniformViewMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	const glm::vec3 pos = m_camera->getViewPosition();
	if (int location = m_particlesProgram.getUniform(UniformViewerPosition); location != -1)
	{
		glUniform3fv(location, 1, glm::value_ptr(pos));
	}
}
