#include "stdafx.h"
#include "Texture2DAtlas.h"
#include "TextureUtils.h"
#include "libplatform/ResourceLoader.h"
#include <codecvt>
#include <cstdlib>
#include <map>
#include <tinyxml2/tinyxml2.h>

namespace xml = tinyxml2;

namespace
{
class CPropertyListParser
{
public:
	using MetaHandler = std::function<void(const std::string &textureName, glm::ivec2 &size)>;
	using FrameHandler = std::function<void(const std::string &, const math::FloatRect &)>;

	CPropertyListParser(const std::string &xmlPath)
		: m_xmlPath(xmlPath)
	{
	}

	void DoOnParsedTextureMeta(const MetaHandler &handler)
	{
		m_onParsedTextureMeta = handler;
	}

	void DoOnParsedFrame(const FrameHandler &handler)
	{
		m_onParsedFrame = handler;
	}

	void Parse()
	{
		const std::string xml = platform::ResourceLoader::loadAsString(m_xmlPath);
		xml::XMLDocument document;
		m_error = document.Parse(xml.c_str(), xml.length());
		CheckError();

		const auto *plist = FindChild(&document, "plist");
		const auto *dict = FindChild(plist, "dict");
		ParseMetadata(GetValueNode(dict, "metadata"));
		ParseFrames(GetValueNode(dict, "frames"));
	}

private:
	const xml::XMLElement *FindChild(const xml::XMLNode *parent, const std::string &name)
	{
		const auto *child = parent->FirstChildElement(name.c_str());
		if (!child)
		{
			throw std::logic_error("Child element '" + name + "' not found in '"
				+ m_xmlPath + "'");
		}
		return child;
	}

	const xml::XMLElement *GetNextSibling(const xml::XMLElement *node)
	{
		const auto *sibling = node->NextSiblingElement();
		if (!sibling)
		{
			std::string name = node->Name();
			throw std::logic_error("Sibling for '" + name + "' not found in '"
				+ m_xmlPath + "'");
		}
		return sibling;
	}

	const xml::XMLElement *GetValueNode(const xml::XMLElement *dict, const std::string &key)
	{
		const auto *pKey = dict->FirstChildElement("key");
		while (pKey)
		{
			std::string currentKey = pKey->GetText();
			if (currentKey == key)
			{
				return GetNextSibling(pKey);
			}
			pKey = pKey->NextSiblingElement("key");
		}

		throw std::logic_error("Key '" + key + "' not found in '"
			+ m_xmlPath + "'");
	}

	void ParseMetadata(const xml::XMLElement *metadata)
	{
		const auto *pString = GetValueNode(metadata, "textureFileName");
		const std::string filename = pString->GetText();

		pString = GetValueNode(metadata, "size");
		const std::string sizeStr = pString->GetText();

		glm::ivec2 size;
		if (sscanf(sizeStr.c_str(), "{%d, %d}", &size.x, &size.y) != size.length())
		{
			throw std::logic_error("Invalid size value '" + sizeStr + "' in '"
				+ m_xmlPath + "'");
		}
		m_onParsedTextureMeta(filename, size);
	}

	void ParseFrames(const xml::XMLElement *frames)
	{
		const auto *pKey = frames->FirstChildElement("key");
		while (pKey)
		{
			const std::string spriteName = pKey->GetText();
			const auto *dict = GetNextSibling(pKey);
			math::FloatRect rect = ParseFrameRect(dict);
			m_onParsedFrame(spriteName, rect);
			pKey = pKey->NextSiblingElement("key");
		}
	}

	math::FloatRect ParseFrameRect(const xml::XMLElement *dict)
	{
		const auto *nodeX = GetValueNode(dict, "x");
		const auto *nodeY = GetValueNode(dict, "y");
		const auto *nodeWidth = GetValueNode(dict, "width");
		const auto *nodeHeight = GetValueNode(dict, "height");

		const auto x = std::stoul(nodeX->GetText());
		const auto y = std::stoul(nodeY->GetText());
		const auto width = std::stoul(nodeWidth->GetText());
		const auto height = std::stoul(nodeHeight->GetText());

		return math::FloatRect(glm::vec2{ float(x), float(y) },
			glm::vec2{ float(x + width), float(y + height) });
	}

	void CheckError()
	{
		if (m_error != xml::XML_SUCCESS)
		{
			const std::string errorCode = std::to_string(unsigned(m_error));
			throw std::runtime_error("Failed to load plist file '" + m_xmlPath
				+ "': xml error #" + errorCode);
		}
	}

	MetaHandler m_onParsedTextureMeta;
	FrameHandler m_onParsedFrame;
	std::string m_xmlPath;
	xml::XMLError m_error = xml::XML_SUCCESS;
};

std::string getParentPath(const std::string &path)
{
	const size_t index = path.find_last_of("/\\");
	if (index != std::string::npos)
	{
		return path.substr(0, index);
	}
	return "";
}
} // namespace

Texture2DAtlas::Texture2DAtlas(const std::string &plistRelativePath)
{
	glm::vec2 frameScale;

	CPropertyListParser parser(plistRelativePath);
	parser.DoOnParsedTextureMeta([&](const std::string &filename, const glm::ivec2 &size) {
		// Запоминаем коэффициенты для преобразования всех координат
		//    в атласе текстур к диапазону [0..1].
		frameScale = { float(1.f / size.x),
			float(1.f / size.y) };
		m_texture = utils::loadTextureFromImage(getParentPath(plistRelativePath) + "/" + filename);
	});
	parser.DoOnParsedFrame([&](const std::string &name, const math::FloatRect &rect) {
		// Преобразуем координаты в атласе текстур к диапазону [0..1]
		FloatRect texRect = rect.getScaled(frameScale);

		// Переворачиваем по оси Y, чтобы синхронизировать
		//    с переворотом текстуры в загузчике текстур.
		const float flippedY = 1.f - texRect.getBottomRight().y;
		texRect.moveTo({ texRect.getTopLeft().x, flippedY });

		m_frames[name] = texRect;
	});
	parser.Parse();
}

gl::GLuint Texture2DAtlas::getTextureId() const
{
	return m_texture;
}

math::FloatRect Texture2DAtlas::getFrameRect(const std::string &frameName) const
{
	try
	{
		return m_frames.at(frameName);
	}
	catch (...)
	{
		throw std::runtime_error("Frame not found: " + frameName);
	}
}
