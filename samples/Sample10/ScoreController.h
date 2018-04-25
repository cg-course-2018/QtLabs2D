#pragma once

class ScoreController
{
public:
	ScoreController();
	~ScoreController();

	void onPairGuessed();
	void onPairMissed();
	void setTileCount(int cnt);
	bool isGameFinished() const;
	int getTileCount() const;
	int getScore() const;

private:
	int m_tilesCount = 0;
	int m_score = 0;
};
