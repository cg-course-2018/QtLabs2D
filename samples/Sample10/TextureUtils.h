#pragma once
#include "libglcore/libglcore.h"

namespace utils
{
// Загружает ресурс из файла в виде текстуры.
// @param relativePath - путь относительно каталога, где лежит исполняемый файл программы.
glcore::TextureObject loadTextureFromImage(const std::string &relativePath);
} // namespace utils
