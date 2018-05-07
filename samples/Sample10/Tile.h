#pragma once

#include "MeshP3N3T2.h"
#include "IShaderProgram.h"
#include "Texture2DAtlas.h"

class TileMesh
{
public:
	TileMesh(const Texture2DAtlas& atlas, const std::string &frontName, const std::string &backName);
	virtual ~TileMesh();

	void initialize(const IShaderProgram& program);
	void update(float deltaSeconds);
	void draw(const IShaderProgram& program);

private:
	enum class State {
		FacedFront,
		Teasing,
		FacedBack,
		Dead
	};

	State m_state = State::FacedFront;
	Transform3D m_transform;
	MeshP3N3T2 m_quad;
	MeshDataP3N3T2 m_quadData;
};