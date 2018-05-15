#pragma once

#include "CurveModel.h"
#include "CurveView.h"
#include "CurveController.h"
#include "ShaderProgram.h"
#include <libglcore/libglcore.h>
#include <libplatform/IRenderScene.h>
#include <memory>

class CurvedScene
    : public platform::IRenderScene
{
public:
    CurvedScene();

    void initialize() final;
    void update(float deltaSeconds) final;
    void redraw(unsigned width, unsigned height) final;
	bool keyPressEvent(platform::IKeyEvent &event) final;
    bool keyReleaseEvent(platform::IKeyEvent &event) final;
    bool mousePressEvent(platform::IMouseEvent &event) final;
    bool mouseMoveEvent(platform::IMouseEvent &event) final;
    bool mouseReleaseEvent(platform::IMouseEvent &event) final;

private:
    void initializeShaders();
    void initializeObjects();
    void setProjectionMatrix(unsigned width, unsigned height);

    glcore::VAO m_vao;
    ShaderProgram m_program2d;
	CurveModel m_curveModel;
	CurveView m_curveView;
    CurveController m_curveController;
};
