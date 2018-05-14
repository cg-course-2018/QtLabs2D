#include "stdafx.h"
#include "ParticlesSceneLoader.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "TextureUtils.h"
#include "libplatform/ResourceLoader.h"
#include "nlohmann/json.hpp"
#include <fstream>

using namespace nlohmann;
using glm::vec2;
using glm::vec3;

namespace
{

std::string getPathDirectory(const std::string &path)
{
	size_t pos = path.find_last_of('/');
	if (pos != std::string::npos)
	{
		return path.substr(0, pos);
	}
	return "";
}

std::string joinPaths(const std::string &head)
{
	return head;
}

template<class... Args>
std::string joinPaths(const std::string &head, Args &&... tail)
{
	return head + "/" + joinPaths(std::forward<Args>(tail)...);
}

// Считывает vec3 в одном из форматов:
//  - число
//  - массив [x, y, z]
vec3 readOptionalVec3(const json &dict, const std::string &key, const vec3 &defaultValue)
{
	vec3 result = defaultValue;

	const auto it = dict.find(key);
	if (it != dict.end())
	{
		const auto &value = *it;
		if (value.is_array())
		{
			result.x = value.at(0).get<float>();
			result.y = value.at(1).get<float>();
			result.z = value.at(2).get<float>();
		}
		if (value.is_number())
		{
			result = vec3(value.get<float>());
		}
	}

	return result;
}

// Считывает диапазон, заданный двумя числами - началом и концом диапазона.
vec2 readRange(const json &dict, const std::string &key)
{
	const auto &value = dict.at(key);
	if (!value.is_array())
	{
		throw std::runtime_error("range value '" + key + "' is not an array");
	}

	vec2 result;
	result.x = value.at(0).get<float>();
	result.y = value.at(1).get<float>();

	if (result.y <= result.x)
	{
		throw std::runtime_error("min <= max in range '" + key + "'");
	}

	return result;
}

class SceneDefinitionParser
{
public:
	SceneDefinitionParser(SceneGraphNodePtr root, std::string workdir)
		: m_root(root)
		, m_workdir(workdir)
	{
	}

	void parseParticleSystems(const json &array)
	{
		for (const json &object : array)
		{
			auto node = loadParticleSystem(object);
			addLocalTransform(node, object);
			m_root->appendChild(node);
		};
	}

private:
	std::shared_ptr<ParticleSystem> loadParticleSystem(const json &dict)
	{
		const vec3 gravity = readOptionalVec3(dict, "gravity", vec3(0));
		const std::string texturePath = joinPaths(m_workdir, dict.at("texture").get<std::string>());
		glcore::TextureObject texture = utils::loadTextureFromImage(texturePath);

		const bool useAdditiveBlending = dict.at("useAdditiveBlending").get<bool>();

		auto pSystem = std::make_shared<ParticleSystem>();
		pSystem->setParticleTexture(std::move(texture));
		pSystem->setEmitter(loadParticleEmitter(dict.at("emitter")));
		pSystem->setGravity(gravity);

		return pSystem;
	}

	std::unique_ptr<ParticleEmitter> loadParticleEmitter(const json &dict)
	{
		const vec3 position = readOptionalVec3(dict, "position", vec3(0, 1, 0));
		const vec3 direction = readOptionalVec3(dict, "direction", vec3(0, 1, 0));
		const float maxDeviationAngle = dict.at("maxDeviationAngle").get<float>();
		const vec2 distanceRange = readRange(dict, "distanceRange");
		const vec2 emitIntervalRange = readRange(dict, "emitIntervalRange");
		const vec2 lifetimeRange = readRange(dict, "lifetimeRange");
		const vec2 speedRange = readRange(dict, "speedRange");

		auto pEmitter = std::make_unique<ParticleEmitter>();
		pEmitter->setPosition(position);
		pEmitter->setDirection(direction);
		pEmitter->setMaxDeviationAngle(maxDeviationAngle);
		pEmitter->setDistanceRange(distanceRange.x, distanceRange.y);
		pEmitter->setEmitIntervalRange(emitIntervalRange.x, emitIntervalRange.y);
		pEmitter->setLifetimeRange(lifetimeRange.x, lifetimeRange.y);
		pEmitter->setSpeedRange(speedRange.x, speedRange.y);

		return pEmitter;
	}

	void addLocalTransform(const SceneGraphNodePtr &node, const json &dict)
	{
		const vec3 size = readOptionalVec3(dict, "size", vec3(1));
		const vec3 position = readOptionalVec3(dict, "position", vec3(0));
		const vec3 eulerAngles = readOptionalVec3(dict, "eulerAngles", vec3(0));

		Transform3D transform;
		transform.m_size = size;
		transform.m_position = position;
		transform.m_orientation = glm::quat(eulerAngles);
		node->setLocalTransform(transform);
	}

	SceneGraphNodePtr m_root;
	std::string m_workdir;
};
} // namespace

SceneGraphNodePtr ParticlesSceneLoader::loadSceneGraph(const std::string &relativePath)
{
	const std::string contentUtf8 = platform::ResourceLoader::loadAsString(relativePath);
	const json sceneObj = json::parse(contentUtf8);

	SceneGraphNodePtr root = std::make_shared<SceneGraphNode>();

	SceneDefinitionParser parser(root, getPathDirectory(relativePath));
	parser.parseParticleSystems(sceneObj["particleSystems"]);

	return root;
}
