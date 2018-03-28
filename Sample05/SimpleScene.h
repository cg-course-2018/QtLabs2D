#pragma once
#include <libplatform/IRenderScene.h>
#include <libplatform/libplatform.h>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
struct VertexP2C4
{
	glm::vec2 xy;
	glm::vec4 rgba;
};

class SimpleScene
	: public platform::IRenderScene
	, private QOpenGLFunctions_3_3_Core
{
public:
	SimpleScene() {}
	~SimpleScene();
	GLuint m_vbo = 0;
	GLuint m_vao = 0;

	GLuint m_vertexShader = 0;
	GLuint m_fragmentShader = 0;
	GLuint m_program = 0;
	
	size_t m_trianglesCount = 0;

	void initialize() final;
	void update(float deltaSeconds) final;
	void redraw(unsigned width, unsigned height) final;
private :
	GLuint compileShader(GLenum type, const std::string & source);
	void initializeShaders();
	GLuint linkProgram(const std::vector<GLuint>& shaders);

	void bindVertexData(const std::vector<VertexP2C4> &verticies);

};