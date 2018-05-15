#include "CurvedScene.h"
#include "libplatform/ResourceLoader.h"
#include <glbinding/gl32core/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Используем функции из gl32core, экспортированные библиотекой glbinding.
using namespace gl32core;

CurvedScene::CurvedScene()
	: m_curveController(m_curveModel, m_curveView)
{
}

void CurvedScene::initialize()
{
	glcore::initGLBinding();

	// Создаём VAO и навсегда переключаемся на него.
	m_vao = glcore::createVAO();
	glBindVertexArray(m_vao);

	initializeShaders();
	initializeObjects();
}

void CurvedScene::update(float deltaSeconds)
{
	(void)deltaSeconds;
}

void CurvedScene::redraw(unsigned width, unsigned height)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	m_program2d.bind();

	// Устанавливаем матрицу ортографического проецирования.
	setProjectionMatrix(width, height);

	m_curveView.draw(m_program2d);
}

bool CurvedScene::keyPressEvent(platform::IKeyEvent &event)
{
	return m_curveController.keyPressEvent(event);
}

bool CurvedScene::keyReleaseEvent(platform::IKeyEvent &event)
{
	return m_curveController.keyReleaseEvent(event);
}

bool CurvedScene::mousePressEvent(platform::IMouseEvent &event)
{
	return m_curveController.mousePressEvent(event);
}

bool CurvedScene::mouseMoveEvent(platform::IMouseEvent &event)
{
	return m_curveController.mouseMoveEvent(event);
}

bool CurvedScene::mouseReleaseEvent(platform::IMouseEvent &event)
{
	return m_curveController.mouseReleaseEvent(event);
}

void CurvedScene::initializeShaders()
{
	const auto vertSource = platform::ResourceLoader::loadAsString("res13/draw2d.vert");
	const auto fragSource = platform::ResourceLoader::loadAsString("res13/draw2d.frag");

	std::vector<glcore::ShaderObject> shaders;
	shaders.emplace_back(glcore::compileShader(GL_VERTEX_SHADER, vertSource));
	shaders.emplace_back(glcore::compileShader(GL_FRAGMENT_SHADER, fragSource));
	auto program = glcore::linkProgram(shaders);

	std::vector<UniformInfo> uniformsMapping = {
		{ UniformProjectionMatrix, "u_projection_matrix" },
		{ UniformCurrentColor, "u_current_color" },
	};
	std::vector<AttributeInfo> attributesMapping = {
		{ AttributePosition, "i_position" },
	};
	m_program2d.init(std::move(program), uniformsMapping, attributesMapping);
}

void CurvedScene::initializeObjects()
{
	constexpr unsigned kTesselatePrecision = 100;
	const glm::vec4 red{ 1, 0, 0, 1 };
	const glm::vec4 yellow{ 1, 1, 0, 1 };

	m_curveView.setPointRadius(10);
	m_curveView.setPointsColor(red);
	m_curveView.setLinesColor(yellow);
	m_curveModel.setPrecision(kTesselatePrecision);
	m_curveModel.setCurveMode(CurveMode::Lines);
	m_curveController.initialize({ { -380, -20 }, { -200, 100 }, { 0, -100 }, { 200, 30 }, { 380, 100 } });
}

void CurvedScene::setProjectionMatrix(unsigned width, unsigned height)
{
	// Вычисляем матрицу ортографического проецирования
	const glm::mat4 mat = glm::ortho(-0.5f * float(width), 0.5f * float(width), -0.5f * float(height), 0.5f * float(height));

	// Передаём матрицу как константу в графической программе
	glUniformMatrix4fv(m_program2d.getUniform(UniformProjectionMatrix), 1, GL_FALSE, glm::value_ptr(mat));
}
