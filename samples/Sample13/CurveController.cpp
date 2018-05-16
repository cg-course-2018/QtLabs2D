#include "stdafx.h"
#include "CurveController.h"
#include "CurveModel.h"
#include "CurveView.h"
#include <glm/geometric.hpp>

namespace
{
glm::vec2 getMousePosition(platform::IMouseEvent &event)
{
	return { event.getPosition().first, event.getPosition().second };
}
} // namespace

CurveController::CurveController(CurveModel &curve, CurveView &curveView)
	: m_curveModel(curve)
	, m_curveView(curveView)
{
}

void CurveController::initialize(std::initializer_list<glm::vec2> controlPoints)
{
	for (glm::vec2 point : controlPoints)
	{
		m_curveModel.addControlPoint(point);
	}
	updateView();
}

bool CurveController::keyReleaseEvent(platform::IKeyEvent &event)
{
	bool handled = false;
	if (event.getKey() == platform::Key::Delete)
	{
		// TODO: добавьте обрезание кривой при нажатии Delete
		//  1) для обрезания используйте метод truncate(...) у модели
		//  2) в качестве нового числа контрольных точек передайте CurveModel::kMinControlPointCount

		m_curveModel.truncate(CurveModel::kMinControlPointCount);

		updateView();
		handled = true;
	}
	else if (event.getKey() == platform::Key::Left)
	{
		// TODO: добавьте переключение режимов тесселяции кривой (CurveMode) в порядке возрастания.
		int mode = static_cast<int>(m_curveModel.getCurveMode());
		mode = (++mode) % 3;
		m_curveModel.setCurveMode(static_cast<CurveMode>(mode));

		updateView();
		handled = true;
	}
	else if (event.getKey() == platform::Key::Right)
	{
		// TODO: добавьте переключение режимов тесселяции кривой (CurveMode) в порядке убывания.
		int mode = static_cast<int>(m_curveModel.getCurveMode());

		mode = (--mode < 0) ? 2 : mode;

		m_curveModel.setCurveMode(static_cast<CurveMode>(mode));

		updateView();
		handled = true;
	}
	return handled;
}

bool CurveController::mousePressEvent(platform::IMouseEvent &event)
{
	bool handled = false;
	if (event.isButtonPressed(platform::MouseButton::Left))
	{
		const glm::vec2 clickPos = getMousePosition(event);

		// Проверяем, не попали ли мы в какую-то точку.
		// Если попали, начинается drag&drop.
		for (size_t i = 0; i < m_curveModel.getControlPointCount(); ++i)
		{
			const float clickDistance = glm::distance(clickPos, m_curveModel.getControlPoint(i));
			if (clickDistance <= m_curveView.getPointRadius())
			{
				m_draggedPointIndex = i;
				handled = true;
				break;
			}
		}

		// Если не попали в какую-либо точку, то, когда кнопка мыши будет отпущена,
		//  надо будет создать новую точку.
		if (!handled)
		{
			m_creationPoint = clickPos;
		}

		handled = true;
	}
	return handled;
}

bool CurveController::mouseMoveEvent(platform::IMouseEvent &event)
{
	bool handled = false;
	// Если выполнялся drag&drop, продолжаем его.
	if (m_draggedPointIndex)
	{
		const glm::vec2 newDragPos = getMousePosition(event);
		m_curveModel.setControlPoint(*m_draggedPointIndex, newDragPos);
		updateView();
		handled = true;
	}
	return handled;
}

bool CurveController::mouseReleaseEvent(platform::IMouseEvent &event)
{
	bool handled = false;
	if (!event.isButtonPressed(platform::MouseButton::Left))
	{
		// Если выполнялся drag&drop, завершаем его.
		if (m_draggedPointIndex)
		{
			const glm::vec2 newDragPos = getMousePosition(event);
			m_curveModel.setControlPoint(*m_draggedPointIndex, newDragPos);
			m_draggedPointIndex = std::nullopt;
			updateView();
			handled = true;
		}
		// Иначе, если раньше был клик и курсор не ушёл слишом далеко, создаём новую точку.
		else if (m_creationPoint)
		{
			const glm::vec2 clickPos = getMousePosition(event);
			const float moveDistance = glm::distance(clickPos, *m_creationPoint);
			if (moveDistance <= m_curveView.getPointRadius())
			{
				m_curveModel.addControlPoint(*m_creationPoint);
				m_creationPoint = std::nullopt;
				updateView();
				handled = true;
			}
		}
	}
	return handled;
}

void CurveController::updateView()
{
	m_curveView.setControlPoints(m_curveModel.getControlPoints());
	m_curveView.setCurvePoints(m_curveModel.tesselate());
}
