#include "stdafx.h"
#include "GraphicsSceneAdapter.h"

namespace platform
{

GraphicsSceneAdapter::GraphicsSceneAdapter(std::unique_ptr<IGraphicsScene> nested)
	: m_nested(std::move(nested))
{
}

GraphicsSceneAdapter::~GraphicsSceneAdapter() = default;

void GraphicsSceneAdapter::update(float deltaSeconds)
{
	m_nested->update(deltaSeconds);
}

void GraphicsSceneAdapter::redraw(unsigned width, unsigned height)
{
	if (!m_device)
	{
		m_device = std::make_unique<QOpenGLPaintDevice>();
	}
	m_device->setSize(QSize(width, height));
	QPainter painter(m_device.get());

	painter.fillRect(0, 0, static_cast<int>(width), static_cast<int>(height), Qt::white);
	m_nested->redraw(painter);
	painter.end();
}

} // namespace platform
