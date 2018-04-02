#pragma once

#include "IGraphicsScene.h"
#include "IRenderScene.h"
#include <QtCore/QElapsedTimer>
#include <QtGui/QExposeEvent>
#include <QtGui/QOpenGLContext>
#include <QtGui/QResizeEvent>
#include <QtGui/QWindow>
#include <memory>

namespace platform
{

struct RenderWindowOptions
{
	unsigned width = 800;
	unsigned height = 600;
	QSurfaceFormat format;
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
	void setScene(std::unique_ptr<IRenderScene> scene);

protected:
	bool event(QEvent *event) override;
	void exposeEvent(QExposeEvent *event) override;

	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;

private:
	void renderLater();
	void renderNow();
	void updateScene();
	void renderScene();

	template<class Callable>
	void CatchAndClose(Callable &&callable);

	RenderWindowOptions m_options;
	QSurfaceFormat m_surfaceFormat;
	std::unique_ptr<IRenderScene> m_scene;
	bool m_isAnimating = true;
	QElapsedTimer m_updateTimer;
	QOpenGLContext *m_context = nullptr;
};

} // namespace platform
