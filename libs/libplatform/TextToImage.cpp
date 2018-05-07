#include "stdafx.h"
#include "TextToImage.h"
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

namespace platform
{

QImage platform::textToImage(const QFont &font, const QString &text)
{
	QFontMetrics metrics(font);
	QSize size{ metrics.width(text), metrics.height() };
	return textToImage(font, size, text);
}

QImage platform::textToImage(const QFont &font, const QSize &size, const QString &text)
{
	QImage image{ size.width(), size.height(), QImage::Format_RGBA8888 };
	image.fill(Qt::white);

	QPainter painter(&image);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setPen(QPen(Qt::black));
	painter.setFont(font);
	painter.drawText(image.rect(), Qt::AlignCenter, text);

	return image;
}

} // namespace platform
