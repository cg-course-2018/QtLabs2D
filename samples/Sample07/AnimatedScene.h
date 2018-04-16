#pragma once
#include "TesselateUtils.h"
#include <libglcore/libglcore.h>
#include <libplatform/IRenderScene.h>

class AnimatedScene
	: public platform::IRenderScene
{
public:
	AnimatedScene();
	~AnimatedScene();

	void initialize() final;
	void update(float deltaSeconds) final;
	void redraw(unsigned width, unsigned height) final;
	bool keyReleaseEvent(platform::IKeyEvent &event) final;

private:
	void bindVertexData(const std::vector<VertexP2C4> &verticies);
	void initializeShaders();
	void setProjectionMatrix(unsigned width, unsigned height);
	void animateShape(std::vector<VertexP2C4> &verticies);
	glm::vec2 animate(const glm::vec2 &point, float phase);

	glcore::ProgramObject m_program;
	glcore::VBO m_vbo;
	glcore::VAO m_vao;

	size_t m_vertexCount = 0;
	float m_totalTime = 0;
};
