#pragma once
#include "libchapter2.h"
#include <vector>

// ������� � ��������� ��������, �������� � 2D ������������ ��������.
struct SVertexP3NT2
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

// ������������� ������������ ������, ������� � ��������� Oxz.
// �������� ������� ����� ������� (0, +1, 0),
// ������ ������� ����� ������� (0, -1, 0).
class CTwoSideQuad : public ISceneObject
{
public:
	CTwoSideQuad(const glm::vec2 &leftTop, const glm::vec2 &size);

	void Update(float) override {}
	void Draw()const override;

	void SetFrontTextureRect(const CFloatRect &rect);
	void SetBackTextureRect(const CFloatRect &rect);

private:
	std::vector<SVertexP3NT2> m_vertices;
	std::vector<uint8_t> m_indicies;
};

enum class TileImage
{
	FLY = 0,
	EXIT_SIGN,
	HEART,
	KEY,
	MUSHROOM,
	SNAIL,
	SPRINGBOARD,
	FISH,

	NUM_TILE_IMAGES,
};

class CAnimationCounter
{
public:
	CAnimationCounter(float changeSpeed);

	float GetPhase()const;
	bool IsActive()const;

	// ������������� ��������� ���� ��������.
	void Restart();

	// ���������� ������� ���� ���� ������ ��������
	//  ����������, ���� ���� �������� 1.
	void Update(float deltaSeconds);

private:
	bool m_isActive = false;
	float m_phase = 1.f;
	float m_changeSpeed = 0;
};

class CMemoryTile : public CTwoSideQuad
{
public:
	CMemoryTile(TileImage tileImage,
		const glm::vec2 &leftTop, const glm::vec2 &size);

	TileImage GetTileImage() const;
	void SetTileImage(TileImage GetTileImage);

	bool IsFrontFaced()const;
	bool IsAlive()const;

	bool MaybeActivate(const glm::vec2 &point);
	void Deactivate();
	void Kill();

	void Update(float dt) override;
	void Draw()const override;//���������� ������ ����� ���������, ����������

private:
	enum class State
	{
		FacedFront,
		Teasing,
		FacedBack,
		Dead,
	};

	TileImage m_tileImage = TileImage::FLY;
	State m_state = State::FacedBack;
	CFloatRect m_bounds;
	CAnimationCounter m_animationCounter;
};
