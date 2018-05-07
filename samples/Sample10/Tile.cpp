#include "stdafx.h"
#include "Tile.h"

TileMesh::TileMesh(const Texture2DAtlas& atlas, const std::string &frontName, const std::string &backName)
{
	auto quadMaterial = std::make_shared<Material>();
	quadMaterial->colorMapId = atlas.getTextureId();
	auto frontRect = atlas.getFrameRect(frontName);
	auto backRect = atlas.getFrameRect(backName);
	m_quadData = tesselateTwoSideQuad(frontRect, backRect, quadMaterial);
}

TileMesh::~TileMesh()
{
}

void TileMesh::initialize(const IShaderProgram& program)
{
	m_quad.init(m_quadData);
	m_quad.bindAttributes(program);

	const float quadRotation = -M_PI_2;
	m_transform.rotateBy(glm::angleAxis(quadRotation, glm::vec3{ 1, 0, 0 }));
	m_quad.setTransform(m_transform);
}

void TileMesh::update(float deltaSeconds)
{
}

void TileMesh::draw(const IShaderProgram& program)
{
	m_quad.updateUniforms(program);
	m_quad.draw();
}