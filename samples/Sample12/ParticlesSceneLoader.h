#pragma once
#include "SceneGraph.h"

class ParticlesSceneLoader
{
public:
	ParticlesSceneLoader() = delete;

	// Загружает сцену из заданного JSON-файла,
	//  возвращает указатель на корневой узел.
	static SceneGraphNodePtr loadSceneGraph(const std::string &relativePath);
};
