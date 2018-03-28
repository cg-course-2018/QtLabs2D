#pragma once
#include <QtGui/QOpenGLFunctions_3_1>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLShaderProgram>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <libplatform/IRenderScene.h>

struct VertexP2C4
{
	glm::vec2 xy;
	glm::vec4 rgba;
};

class SimpleScene
	: public platform::IRenderScene,
	  private QOpenGLFunctions_3_1
{
public:
	SimpleScene();
	~SimpleScene();

	void initialize() final;
	void update(float deltaSeconds) final;
	void redraw(unsigned width, unsigned height) final;

private:
	void bindVertexData(const std::vector<VertexP2C4> &verticies);
	void initializeShaders();
	void setProjectionMatrix(unsigned width, unsigned height);

	GLuint compileShader(GLenum type, const std::string& source);
	GLuint linkProgram(const std::vector<GLuint>& shaders);

	GLuint m_vertexShader = 0;
	GLuint m_fragmentShader = 0;
	GLuint m_program = 0;
	GLuint m_vbo = 0;
	GLuint m_vao = 0;
	size_t m_trianglesCount = 0;
};
