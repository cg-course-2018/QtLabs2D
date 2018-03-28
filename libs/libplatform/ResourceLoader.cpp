#include "ResourceLoader.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

namespace
{
}

namespace platform
{
std::string ResourceLoader::loadAsString(const std::string &relativePath)
{
	// Формируем абсолютный путь к файлу, добавляя путь к каталогу исполняемого файла
	QString absolutePath = QCoreApplication::applicationDirPath() + QDir::separator() + QString::fromUtf8(relativePath.c_str());

	// Открываем файл и бросаем исключение, если открыть не удалось.
	QFile file(absolutePath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		throw std::runtime_error("cannot open " + absolutePath.toStdString());
	}

	// Читаем все данные из файла как массив байт.
	QTextStream in(&file);
	QByteArray bytes = in.readAll().toUtf8();

	return std::string(bytes.data(), bytes.size());
}
} // namespace platform
