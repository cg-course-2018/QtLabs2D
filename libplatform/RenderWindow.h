#pragma once

#include "IGraphicsScene.h"
#include <QtCore/QElapsedTimer>
#include <QtGui/QExposeEvent>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QResizeEvent>
#include <QtGui/QWindow>
#include <memory>

namespace platform
{

struct RenderWindowOptions
{
	unsigned width = 800;
	unsigned height = 600;
	QColor clearColor = QColor(0xFF, 0xFF, 0xFF);
};

// Класс RenderWindow реализует окно с постоянной перерисовкой содержимого,
//  подходящее для экспериментов с OpenGL.
class RenderWindow : public QWindow
{
	Q_OBJECT
public:
	explicit RenderWindow(
		const RenderWindowOptions &options = RenderWindowOptions(),
		QWindow *parent = nullptr);
	~RenderWindow();

	bool isAnimating() const;
	void setAnimating(bool isAnimating);

	void setScene(std::unique_ptr<IGraphicsScene> scene);

protected:
	bool event(QEvent *event) override;
	void exposeEvent(QExposeEvent *event) override;

private:
	void renderLater();
	void renderNow();
	void updateScene();
	void renderScene();

	RenderWindowOptions m_options;
	bool m_isAnimating = true;
	QElapsedTimer m_updateTimer;
	QOpenGLContext *m_context = nullptr;
	std::unique_ptr<QOpenGLPaintDevice> m_device = nullptr;
	std::unique_ptr<IGraphicsScene> m_scene;
};

} // namespace platform
