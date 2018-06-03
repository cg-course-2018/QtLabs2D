#include "stdafx.h"
#include "SceneGraph.h"

void SceneGraphNode::update(float deltaSeconds)
{
	for (auto &&child : m_children)
	{
		child->update(deltaSeconds);
	}
}

void SceneGraphNode::draw(const RenderContext &context)
{
	for (auto &&child : m_children)
	{
		RenderContext childContext{ context };
		childContext.parentWorldMat4 = context.parentWorldMat4 * getLocalTransform().toMat4();
		child->draw(childContext);
	}
}

Transform3D SceneGraphNode::getLocalTransform() const
{
	return m_localTransform;
}

void SceneGraphNode::setLocalTransform(const Transform3D &transform)
{
	m_localTransform = transform;
}

void SceneGraphNode::appendChild(SceneGraphNodePtr ptr)
{
	m_children.emplace_back(std::move(ptr));
}
