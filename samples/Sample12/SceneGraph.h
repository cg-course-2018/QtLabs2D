#pragma once

#include "IShaderProgram.h"
#include "libmath/Transform3D.h"
#include <glm/mat4x4.hpp>
#include <memory>
#include <utility>
#include <vector>

struct RenderContext
{
	explicit RenderContext(IShaderProgram &program)
		: program(program)
	{
	}

	std::reference_wrapper<const IShaderProgram> program;
	glm::mat4 worldMat4{ 1 };
	glm::mat4 viewMat4{ 1 };
};

class SceneGraphNode;
using SceneGraphNodePtr = std::shared_ptr<SceneGraphNode>;

class SceneGraphNode
{
public:
	virtual ~SceneGraphNode() = default;

	virtual void update(float deltaSeconds);
	virtual void draw(const RenderContext &context);

	Transform3D getLocalTransform() const;
	void setLocalTransform(const Transform3D &transform);

	void appendChild(SceneGraphNodePtr ptr);

private:
	Transform3D m_localTransform;
	SceneGraphNode *m_parent = nullptr;
	std::vector<SceneGraphNodePtr> m_children;
};
