#include "stdafx.h"
#include "NurbsScene.h"
#include "CameraController.h"
#include "FlyingCamera.h"
#include "MeshP3N3.h"
#include "TesselateUtils.h"
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
const glm::vec3 CAMERA_POSITION = { 0, 12, -15 };
const glm::vec3 CAMERA_TARGET = { 0, 4, 0 };
const glm::vec3 CAMERA_UP = { 0, 1, 0 };
const float SPHERE_ROTATE_DEGREES_PER_SECOND = 90.f;

} // namespace

NurbsScene::NurbsScene()
	: m_camera(std::make_unique<FlyingCamera>(CAMERA_POSITION, CAMERA_TARGET, CAMERA_UP))
	, m_cameraController(std::make_unique<CameraController>(*m_camera))
{
}

NurbsScene::~NurbsScene() = default;

void NurbsScene::initialize()
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
}

void NurbsScene::update(float deltaSeconds)
{
	m_cameraController->update(deltaSeconds);
	if (m_teapotNode)
	{
		m_teapotNode->update(deltaSeconds);
	}

	Transform3D transform = m_teapotNode->getLocalTransform();
	transform.rotateBy(glm::angleAxis(glm::radians(SPHERE_ROTATE_DEGREES_PER_SECOND * deltaSeconds), glm::vec3{0, 0, 1}));
	m_teapotNode->setLocalTransform(transform);
}

void NurbsScene::redraw(unsigned width, unsigned height)
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

	if (m_teapotNode)
	{
		m_teapotNode->draw(ctx);
	}
}

bool NurbsScene::keyPressEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyPressEvent(event);
}

bool NurbsScene::keyReleaseEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyReleaseEvent(event);
}

bool NurbsScene::mousePressEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mousePressEvent(event);
}

bool NurbsScene::mouseMoveEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseMoveEvent(event);
}

bool NurbsScene::mouseReleaseEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseReleaseEvent(event);
}

void NurbsScene::initializeShaders()
{
	initializePhongProgram();
}

void NurbsScene::initializePhongProgram()
{
	platform::ResourceLoader loader;
	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::compileShader(GL_VERTEX_SHADER, loader.loadAsString("res14/phong_lighting.vert")));
	shaders.emplace_back(glcore::compileShader(GL_FRAGMENT_SHADER, loader.loadAsString("res14/phong_lighting.frag")));
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
		{ UniformViewerPosition, "u_viewer_position" },
		{ UniformLight0Position, "u_light0.position" },
		{ UniformLight0Diffuse, "u_light0.diffuse" },
		{ UniformLight0Specular, "u_light0.specular" },
		{ UniformMaterialEmission, "u_material.emission" },
		{ UniformMaterialDiffuse, "u_material.diffuse" },
		{ UniformMaterialSpecular, "u_material.specular" },
	};

	m_particlesProgram.init(std::move(program), uniforms, attributes);
}

void NurbsScene::initializeLights()
{
	glm::vec4 sunDiffuse{ 1, 1, 1, 1 };
	glm::vec4 sunSpecular{ 1, 1, 1, 1 };
	glm::vec3 sunDirection{ 0, 1, 0 };
	m_sunlight = utils::makeDirectedLightSource(sunDirection, sunDiffuse, sunSpecular);
}

void NurbsScene::initializeObjects()
{
	const Material sphereMat{
		glm::vec4{ 0.4, 0.4, 0.4, 1.0 },
		glm::vec4{ 0.5, 0.5, 1.0, 1.0 },
		glm::vec4{ 0.5, 0.5, 1.0, 1.0 }
	};

	const MeshDataP3N3 data = utils::tesselateTeapot(sphereMat, 20, 20);

	auto mesh = std::make_shared<MeshP3N3>();
	mesh->init(data);
	m_teapotNode = mesh;

	Transform3D teapotTransform;
	teapotTransform.scaleBy(3.0f);
	teapotTransform.rotateBy(glm::angleAxis(glm::radians(-90.f), glm::vec3{ 1, 0, 0 }));
	m_teapotNode->setLocalTransform(teapotTransform);
}

void NurbsScene::setProjectionMatrix(unsigned width, unsigned height)
{
	// Вычисляем матрицу перспективного проецирования.
	// Затем передаём матрицу как константу в графической программе.
	const float fieldOfView = glm::radians(70.f);
	const float aspect = float(width) / float(height);
	const float zNear = 0.5f;
	const float zFar = 50.f;
	const glm::mat4 mat = glm::perspective(fieldOfView, aspect, zNear, zFar);

	if (int location = m_particlesProgram.getUniform(UniformProjectionMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
}

void NurbsScene::setCameraUniforms()
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
