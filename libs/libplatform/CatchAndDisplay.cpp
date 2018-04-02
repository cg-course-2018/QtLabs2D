#include "CatchAndDisplay.h"
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

void platform::detail::DisplayException(const std::exception &ex)
{
	qDebug() << QString::fromUtf8(ex.what());
	QMessageBox::warning(
		nullptr,
		QLatin1String("Exception Caught"),
		QString::fromUtf8(ex.what()));
}
