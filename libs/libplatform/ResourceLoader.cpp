#include "ResourceLoader.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

namespace
{
QString findResource(const std::string &relativePath)
{
	// Конвертируем относительный путь в QString, заменяем прямой/обратный слеш на платформо-зависимый разделитель путей.
	QString cleanRelativePath = QString::fromUtf8(relativePath.c_str());
	if (QDir::separator() != QLatin1Char('/'))
	{
		cleanRelativePath.replace(QLatin1Char('/'), QDir::separator());
	}

	// Формируем абсолютный путь к файлу, добавляя путь к каталогу исполняемого файла
	return QCoreApplication::applicationDirPath() + QDir::separator() + cleanRelativePath;
}
}

namespace platform
{
std::string ResourceLoader::loadAsString(const std::string &relativePath)
{
	// Получаем абсолютный путь.
	QString absolutePath = findResource(relativePath);

	// Открываем файл и бросаем исключение, если открыть не удалось.
	QFile file(absolutePath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		throw std::runtime_error("cannot open " + absolutePath.toStdString());
	}

	// Читаем все данные из файла как массив байт.
	QTextStream in(&file);
	QByteArray bytes = in.readAll().toUtf8();

	return std::string(bytes.data(), static_cast<size_t>(bytes.size()));
}

QImage ResourceLoader::loadImage(const std::string &relativePath)
{
	// Получаем абсолютный путь.
	QString absolutePath = findResource(relativePath);

	// Конструируем изображение, что приведёт к его загрузке.
	QImage image;
	if (!image.load(absolutePath))
	{
		throw std::runtime_error("cannot load image " + relativePath);
	}
	return image;
}
} // namespace platform
