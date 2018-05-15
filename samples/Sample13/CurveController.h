#include "libplatform/IInputListener.h"
#include <optional>
#include <glm/vec2.hpp>

class CurveModel;
class CurveView;

class CurveController : public platform::IInputListener
{
public:
	CurveController(CurveModel& curve, CurveView& curveView);

	void initialize(std::initializer_list<glm::vec2> controlPoints);

    bool keyReleaseEvent(platform::IKeyEvent &event) final;
    bool mousePressEvent(platform::IMouseEvent &event) final;
    bool mouseMoveEvent(platform::IMouseEvent &event) final;
    bool mouseReleaseEvent(platform::IMouseEvent &event) final;

private:
	void resetCurve();
	void updateView();

	CurveModel& m_curveModel;
	CurveView& m_curveView;
    std::optional<size_t> m_draggedPointIndex;
    std::optional<glm::vec2> m_creationPoint;
};
