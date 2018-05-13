#include "stdafx.h"
#include "SceneGraph.h"

void SceneGraphNode::update(float deltaSeconds)
{
    for (auto&& child : m_children)
    {
        child->update(deltaSeconds);
    }
}

void SceneGraphNode::draw(const RenderContext& context)
{
    RenderContext childrenContext{ context };
    childrenContext.worldMat4 = childrenContext.worldMat4 * getLocalTransform().toMat4();
    for (auto&& child : m_children)
    {
        child->draw(childrenContext);
    }
}

Transform3D SceneGraphNode::getLocalTransform() const
{
    return m_localTransform;
}

void SceneGraphNode::setLocalTransform(const Transform3D& transform)
{
    m_localTransform = transform;
}

void SceneGraphNode::appendChild(SceneGraphNodePtr ptr)
{
    m_children.emplace_back(std::move(ptr));
}
