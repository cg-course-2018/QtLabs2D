#pragma once

#include <QtGui/QImage>

namespace platform
{

class ResourceLoader
{
public:
	// Загружает ресурс из файла в виде строки.
	// @param relativePath - путь относительно каталога, где лежит исполняемый файл программы.
	static std::string loadAsString(const std::string &relativePath);

	// Загружает ресурс из файла в виде изображения.
	// @param relativePath - путь относительно каталога, где лежит исполняемый файл программы.
	static QImage loadImage(const std::string &relativePath);
};

} // namespace platform
