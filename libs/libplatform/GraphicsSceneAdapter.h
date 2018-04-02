#pragma once

#include "IGraphicsScene.h"
#include "IRenderScene.h"
#include <QtGui/QOpenGLPaintDevice>
#include <memory>

namespace platform
{

// Адаптирует объект, реалзующий интерфейс IGraphicsScene,
//  для использования по интерфейсу IRenderScene
class GraphicsSceneAdapter final : public IRenderScene
{
public:
	GraphicsSceneAdapter(std::unique_ptr<IGraphicsScene> nested);
	~GraphicsSceneAdapter();

	void update(float deltaSeconds) final;
	void redraw(unsigned width, unsigned height) final;

private:
	std::unique_ptr<IGraphicsScene> m_nested;
	std::unique_ptr<QOpenGLPaintDevice> m_device = nullptr;
};

} // namespace platform
