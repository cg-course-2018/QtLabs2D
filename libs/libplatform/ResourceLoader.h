#pragma once

namespace platform
{

class ResourceLoader
{
public:
	// Загружает ресурс из файла в виде строки.
	// @param relativePath - путь относительно каталога, где лежит исполняемый файл программы.
	static std::string loadAsString(const std::string &relativePath);
};

} // namespace platform
