#pragma once
#include <libglcore/libglcore.h>
#include <libmath/FloatRect.h>
#include <memory>

class Texture2DAtlas;
using Texture2DAtlasPtr = std::shared_ptr<Texture2DAtlas>;

/// ����� ������ ����� �������,
///    ������� ������������ �� ���� ��������� �� ���������� �������
///    ������ ��������, � ����� ������ ������ ��� ���������� ����
///    �������� �� �������������� � ������� ���������� ���������.
/// ������ ������� ��������� � cocos2d-x, � ����� ���� ������
///    � ������� github.com/sergey-shambir/Cheetah-Texture-Packer
class Texture2DAtlas
{
public:
	Texture2DAtlas(const std::string &plistRelativePath);

	gl::GLuint getTextureId() const;
	math::FloatRect getFrameRect(const std::string &frameName) const;

private:
	std::unordered_map<std::string, math::FloatRect> m_frames;
	glcore::TextureObject m_texture;
};
