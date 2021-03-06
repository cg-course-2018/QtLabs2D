#include "stdafx.h"
#include "TextureUtils.h"
#include "libplatform/ResourceLoader.h"
#include "libplatform/TextToImage.h"
#include <glbinding/gl32core/gl.h>

// Используем функции из gl32core, экспортированные библиотекой glbinding.
using namespace gl32core;

namespace
{
glcore::TextureObject makeTextureFromImage(QImage image)
{
	if (image.format() != QImage::Format_RGBA8888)
	{
		image = image.convertToFormat(QImage::Format_RGBA8888);
	}
	// В OpenGL и в Qt системы координат зеркальны относительно оси OX,
	//  поэтому мы перевернём изображение.
	image = image.mirrored(false, true);

	// Просим драйвер создать текстурный объект.
	glcore::TextureObject texture = glcore::createTexture();

	// Привязываем текстурук к текущему контексту.
	glBindTexture(GL_TEXTURE_2D, texture);

	// Генерируем mimpap уровни текстуры.
	// Это поможет OpenGL быстро и качественно отображать текстуру с разным масштабом.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Задаём обрезание картинки при выходе текстурных координат за пределы [0..1]
	// См. также https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Задаём уровни фильтрации
	// См. также http://www.learnopengles.com/android-lesson-six-an-introduction-to-texture-filtering/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	const auto width = static_cast<GLsizei>(image.width());
	const auto height = static_cast<GLsizei>(image.height());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

	return texture;
}

QString fromUtf8(const std::string &textUtf8)
{
	return QString::fromUtf8(textUtf8.data(), static_cast<int>(textUtf8.size()));
}
} // namespace

glcore::TextureObject utils::loadTextureFromImage(const std::string &relativePath)
{
	// Загружаем изображение и приводим его к формату ARGB (8 бит на цветовой канал)
	QImage image = platform::ResourceLoader::loadImage(relativePath);
	return makeTextureFromImage(std::move(image));
}

glcore::TextureObject utils::renderTextAsTexture(int fontSize, const std::string &textUtf8)
{
	// Растеризуем изображение
	QFont textFont(QLatin1Literal("Arial"), fontSize);
	QImage image = platform::textToImage(textFont, fromUtf8(textUtf8));
	return makeTextureFromImage(std::move(image));
}

glcore::TextureObject utils::renderTextAsTexture(const glm::uvec2 &size, int fontSize, const std::string &textUtf8)
{
	QFont textFont(QLatin1Literal("Arial"), fontSize);
	QImage image = platform::textToImage(textFont, QSize(size.x, size.y), fromUtf8(textUtf8));

	return makeTextureFromImage(std::move(image));
}
