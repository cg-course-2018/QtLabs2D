#pragma once
#include "libglcore/libglcore.h"
#include <glm/vec2.hpp>

namespace utils
{

// Загружает ресурс из файла в виде текстуры.
// @param relativePath - путь относительно каталога, где лежит исполняемый файл программы.
glcore::TextureObject loadTextureFromImage(const std::string &relativePath);

// Растеризует текст в изображение, затем загружает в текстуру
// @param textUtf8 - текст в кодировке UTF-8
glcore::TextureObject renderTextAsTexture(int fontSize, const std::string &textUtf8);

// Растеризует текст в изображение, затем загружает в текстуру
// @param size - размер текста в пикселах, текст центрируется по вертикали
//  и горизонтали в пределах заданного размерами прямоугольника.
// @param textUtf8 - текст в кодировке UTF-8
glcore::TextureObject renderTextAsTexture(const glm::uvec2 &size, int fontSize, const std::string &textUtf8);

} // namespace utils
