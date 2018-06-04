#include "stdafx.h"
#include "PointCloudScene.h"
#include "CameraController.h"
#include "FlyingCamera.h"
#include "MeshP3C3N3.h"
#include "PointCloudUtils.h"
#include "TesselateUtils.h"
#include <algorithm>
#include <glbinding/gl32core/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <libplatform/ResourceLoader.h>

// Используем функции из gl32core, экспортированные библиотекой glbinding.
using namespace gl32core;
using namespace glm;

namespace
{

// Три вектора, описывающих первоначальное положение камеры:
//  - положение камеры
//  - положение цели, на которую направлена камера
//  - направление "вверх" для камеры
const vec3 CAMERA_POSITION = { 0, 2, -4 };
const vec3 CAMERA_TARGET = { 0, 0, 0 };
const vec3 CAMERA_UP = { 0, 1, 0 };
const float MESH_ROTATE_DEGREES_PER_SECOND = 30.f;

constexpr float DEBUG_POINT_SIZE = 10.f;

} // namespace

PointCloudScene::PointCloudScene()
	: m_camera(std::make_unique<FlyingCamera>(CAMERA_POSITION, CAMERA_TARGET, CAMERA_UP))
	, m_cameraController(std::make_unique<CameraController>(*m_camera))
{
}

PointCloudScene::~PointCloudScene() = default;

void PointCloudScene::initialize()
{
	glcore::initGLBinding();

	// Создаём VAO и навсегда переключаемся на него.
	m_vao = glcore::createVAO();
	glBindVertexArray(m_vao);

	initializeShaders();
	initializeLights();
	initializeObjects();

	// Увеличиваем размер точек, нарисованных через примитив GL_POINTS.
	glPointSize(DEBUG_POINT_SIZE);

	// Включаем тест глубины.
	glEnable(GL_DEPTH_TEST);

	// TODO: sergey.shambir update this code or remove it
#if 0
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// TODO: sergey.shambir update this code or remove it
#if 0
	// Включаем отсечение задних граней
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
#endif
}

void PointCloudScene::update(float deltaSeconds)
{
	m_cameraController->update(deltaSeconds);
	if (m_leftSurfaceMesh)
	{
		m_leftSurfaceMesh->update(deltaSeconds);
	}
	if (m_rightSurfaceMesh)
	{
		m_rightSurfaceMesh->update(deltaSeconds);
	}

	// TODO: sergey.shambir update this code or remove it
#if 0
	Transform3D transform = m_surfaceMesh->getLocalTransform();
	transform.rotateBy(angleAxis(radians(MESH_ROTATE_DEGREES_PER_SECOND * deltaSeconds), vec3{ 0, 0, 1 }));
	m_surfaceMesh->setLocalTransform(transform);
#endif
}

void PointCloudScene::redraw(unsigned width, unsigned height)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	m_phongProgram.bind();
	utils::setLightSource0(m_phongProgram, m_sunlight);

	// Устанавливаем матрицу ортографического проецирования.
	setProjectionMatrix(width, height);

	// Устанавливаем uniform-переменные шейдера в соответствие с состоянием камеры .
	setCameraUniforms();

	RenderContext ctx{ m_phongProgram };
	ctx.viewMat4 = m_camera->getViewTransform();

	if (m_leftSurfaceMesh)
	{
		m_leftSurfaceMesh->draw(ctx);
	}
	if (m_rightSurfaceMesh)
	{
		m_rightSurfaceMesh->draw(ctx);
	}
}

bool PointCloudScene::keyPressEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyPressEvent(event);
}

bool PointCloudScene::keyReleaseEvent(platform::IKeyEvent &event)
{
	return m_cameraController->keyReleaseEvent(event);
}

bool PointCloudScene::mousePressEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mousePressEvent(event);
}

bool PointCloudScene::mouseMoveEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseMoveEvent(event);
}

bool PointCloudScene::mouseReleaseEvent(platform::IMouseEvent &event)
{
	return m_cameraController->mouseReleaseEvent(event);
}

void PointCloudScene::initializeShaders()
{
	initializePhongProgram();
}

void PointCloudScene::initializePhongProgram()
{
	platform::ResourceLoader loader;
	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::compileShader(GL_VERTEX_SHADER, loader.loadAsString("res16/phong_lighting.vert")));
	shaders.emplace_back(glcore::compileShader(GL_FRAGMENT_SHADER, loader.loadAsString("res16/phong_lighting.frag")));
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

	m_phongProgram.init(std::move(program), uniforms, attributes);
}

void PointCloudScene::initializeLights()
{
	vec4 sunDiffuse{ 1, 1, 1, 1 };
	vec4 sunSpecular{ 1, 1, 1, 1 };
	vec3 sunDirection{ 0, 1, 0 };
	m_sunlight = utils::makeDirectedLightSource(sunDirection, sunDiffuse, sunSpecular);
}

void PointCloudScene::initializeObjects()
{
	const Material meshMat{
		vec4{ 0.2, 0.2, 0.1, 1.0 },
		vec4{ 0.9, 0.9, 0.0, 1.0 },
		vec4{ 0.5, 0.5, 0.5, 1.0 }
	};

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr sourceCloud = utils::loadPointCloud("res16/bunny.pcd");

	{
		pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr leftCloudWithNormals = utils::calculatePointCloudNormals(sourceCloud);

		// TODO: (cg16.1) раскомментировать инициализацию через makeGreedyProjectionTriangulation
#if 0
		const MeshDataP3C3N3 data = utils::makeGreedyProjectionTriangulation(meshMat, leftCloudWithNormals);
#else
		const MeshDataP3C3N3 data = utils::makeMeshFromPoints(meshMat, leftCloudWithNormals);
#endif

		auto mesh = std::make_shared<MeshP3C3N3>();
		mesh->init(data);
		m_leftSurfaceMesh = mesh;

		Transform3D meshTransform;
		meshTransform.scaleBy(10.0f);
		meshTransform.moveBy(vec3{ 1, 0, 0 });
		meshTransform.rotateBy(angleAxis(radians(180.f), vec3{ 0, 1, 0 }));
		m_leftSurfaceMesh->setLocalTransform(meshTransform);
	}

	// TODO: (cg16.2) раскомментировать инициализацию
#if 0
	{
		pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr rightCloudWithNormals = utils::calculatePointCloudNormals2(utils::makeVoxelGrid(sourceCloud));
		const MeshDataP3C3N3 data = utils::makeGreedyProjectionTriangulation(meshMat, rightCloudWithNormals);
		auto mesh = std::make_shared<MeshP3C3N3>();
		mesh->init(data);
		m_rightSurfaceMesh = mesh;

		Transform3D meshTransform;
		meshTransform.scaleBy(10.0f);
		meshTransform.moveBy(vec3{ -1, 0, 0 });
		meshTransform.rotateBy(angleAxis(radians(180.f), vec3{ 0, 1, 0 }));
		m_rightSurfaceMesh->setLocalTransform(meshTransform);
	}
#endif
}

void PointCloudScene::setProjectionMatrix(unsigned width, unsigned height)
{
	// Вычисляем матрицу перспективного проецирования.
	// Затем передаём матрицу как константу в графической программе.
	const float fieldOfView = radians(70.f);
	const float aspect = float(width) / float(height);
	const float zNear = 0.5f;
	const float zFar = 50.f;
	const mat4 mat = perspective(fieldOfView, aspect, zNear, zFar);

	if (int location = m_phongProgram.getUniform(UniformProjectionMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(mat));
	}
}

void PointCloudScene::setCameraUniforms()
{
	const mat4 mat = m_camera->getViewTransform();
	if (int location = m_phongProgram.getUniform(UniformViewMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(mat));
	}

	const vec3 pos = m_camera->getViewPosition();
	if (int location = m_phongProgram.getUniform(UniformViewerPosition); location != -1)
	{
		glUniform3fv(location, 1, value_ptr(pos));
	}
}
