#pragma once

#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QFont>
#include <QtGui/QImage>

namespace platform
{

/// Растеризует текст в изображение.
/// Размер изображения подбирается таким, чтобы он мог вместить текст.
/// @param textUtf8 - текст для растеризаци
/// @param font - шрифт, используемый при растеризации
QImage textToImage(const QFont &font, const QString &text);

// Растеризует текст в изображение.
/// @param textUtf8 - текст для растеризации
/// @param font - шрифт, используемый при растеризации
/// @param size - размеры изображения (текст рисуется в центре по вертикали и по горизонтали)
QImage textToImage(const QFont &font, const QSize &size, const QString &text);

} // namespace platform
