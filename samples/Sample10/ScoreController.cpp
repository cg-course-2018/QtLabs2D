#include "stdafx.h"
#include "ScoreController.h"

namespace
{
constexpr int PAIRGUESSEDSCORE = 10;
constexpr int PAIRMISSEDSCORE = 1;
} // namespace

//Constructor
ScoreController::ScoreController()
{
}

ScoreController::~ScoreController()
{
}

//Methods
void ScoreController::onPairGuessed()
{
	m_score += PAIRGUESSEDSCORE;
	m_tilesCount -= 2;
}

void ScoreController::onPairMissed()
{
	m_score = std::max(0, m_score - PAIRMISSEDSCORE);
}

void ScoreController::setTileCount(int cnt)
{
	m_tilesCount = cnt;
}

bool ScoreController::isGameFinished() const
{
	return m_tilesCount == 0;
}

int ScoreController::getTileCount() const
{
	return m_tilesCount;
}

int ScoreController::getScore() const
{
	return m_score;
}
