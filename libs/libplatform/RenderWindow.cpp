#include "RenderWindow.h"
#include "CatchAndDisplay.h"
#include "GraphicsSceneAdapter.h"
#include "InputListenerAdapter.h"
#include "SetupDebugOutput.h"
#include <QtCore/QDebug>

namespace platform
{
namespace
{
// Интервал не может быть больше указанного,
//  чтобы избежать скачков симуляции при переводе часов.
constexpr float MAX_ELAPSED_SECONDS = 0.1f;

QString formatProfile(QSurfaceFormat::OpenGLContextProfile profile)
{
	switch (profile)
	{
	default:
	case QSurfaceFormat::NoProfile:
		return "no profile";
	case QSurfaceFormat::CoreProfile:
		return "core profile";
	case QSurfaceFormat::CompatibilityProfile:
		return "compatibility profile";
	}
}

QString formatSurfaceFormat(const QSurfaceFormat &format)
{
	QString result;
	result += formatProfile(format.profile());
	result += QLatin1Literal(", version = ");
	result += QString::number(format.majorVersion());
	result += QLatin1Literal(".");
	result += QString::number(format.minorVersion());

	return result;
}
} // namespace

RenderWindow::RenderWindow(const RenderWindowOptions &options, QWindow *parent)
	: QWindow(parent)
	, m_options(options)
	, m_surfaceFormat(options.format)
{
	// Устанавливаем минимальные размеры.
	setMinimumSize(QSize(options.width, options.height));

	// Используем OpenGL как средство рендеринга.
	setSurfaceType(QWindow::OpenGLSurface);

	// Уставливаем параметры контекста OpenGL.
	setFormat(m_surfaceFormat);
	QSurfaceFormat::setDefaultFormat(m_surfaceFormat);

	// Совершаем первый сброс таймера обновления симуляции.
	m_updateTimer.start();
}

RenderWindow::~RenderWindow() = default;

bool RenderWindow::isAnimating() const
{
	return m_isAnimating;
}

void RenderWindow::setAnimating(bool isAnimating)
{
	m_isAnimating = isAnimating;
	if (isAnimating)
	{
		renderLater();
	}
}

void RenderWindow::setScene(std::unique_ptr<IGraphicsScene> scene)
{
	m_scene = std::make_unique<GraphicsSceneAdapter>(std::move(scene));
}

void RenderWindow::setScene(std::unique_ptr<IRenderScene> scene)
{
	m_scene = std::move(scene);
}

bool RenderWindow::event(QEvent *event)
{
	if (event->type() == QEvent::UpdateRequest)
	{
		renderNow();
		return true;
	}
	return QWindow::event(event);
}

void RenderWindow::exposeEvent(QExposeEvent *event)
{
	if (isExposed())
	{
		renderNow();
	}
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
	if (m_scene)
	{
		InputListenerAdapter listener(*m_scene);
		if (listener.keyPressEvent(event))
		{
			return;
		}
	}
	QWindow::keyPressEvent(event);
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
	if (m_scene)
	{
		InputListenerAdapter listener(*m_scene);
		if (listener.keyReleaseEvent(event))
		{
			return;
		}
	}
	QWindow::keyReleaseEvent(event);
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
	if (m_scene)
	{
		InputListenerAdapter listener(*m_scene);
		if (listener.mousePressEvent(event))
		{
			return;
		}
	}
	QWindow::mousePressEvent(event);
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_scene)
	{
		InputListenerAdapter listener(*m_scene);
		if (listener.mouseReleaseEvent(event))
		{
			return;
		}
	}
	QWindow::mouseReleaseEvent(event);
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
	if (m_scene)
	{
		InputListenerAdapter listener(*m_scene);
		if (listener.mouseMoveEvent(event))
		{
			return;
		}
	}
	QWindow::mouseMoveEvent(event);
}

void RenderWindow::renderLater()
{
	requestUpdate();
}

void RenderWindow::renderNow()
{
	if (!isExposed())
	{
		return;
	}

	// Лениво инициализируем контекст OpenGL.
	if (!m_context)
	{
		m_context = new QOpenGLContext(this);
		m_context->setFormat(m_surfaceFormat);
		m_context->create();
		m_context->makeCurrent(this);

		QSurfaceFormat actualFormat = m_context->format();
		qDebug() << formatSurfaceFormat(actualFormat);

		if (m_scene)
		{
			CatchAndClose([&] {
				m_scene->initialize();
			});
		}

		// Если установлен флаг отладочного контекста, подключаем наш callback для отладки.
		if (m_surfaceFormat.testOption(QSurfaceFormat::DebugContext))
		{
			SetupDebugOutput();
		}
	}
	else
	{
		m_context->makeCurrent(this);
	}

	if (m_scene)
	{
		updateScene();
		renderScene();
	}
	// Выполняем обмен буферами в рамках двойной буферизации рисования.
	m_context->swapBuffers(this);

	if (m_isAnimating)
	{
		renderLater();
	}
}

void RenderWindow::updateScene()
{
	const float elapsedSeconds = float(m_updateTimer.elapsed()) / 1000.f;

	// Пропуск обновления в случае, если таймер не успел засечь прошедшее время.
	if (elapsedSeconds > 0)
	{
		m_updateTimer.restart();
		CatchAndClose([&] {
			m_scene->update((std::min)(elapsedSeconds, MAX_ELAPSED_SECONDS));
		});
	}
}

void RenderWindow::renderScene()
{
	CatchAndClose([&] {
		m_scene->redraw(width(), height());
	});
}

template<class Callable>
void RenderWindow::CatchAndClose(Callable &&callable)
{
	const bool ok = CatchAndDisplay([&] {
		callable();
		return true;
	});
	if (!ok)
	{
		close();
	}
}

} // namespace platform
