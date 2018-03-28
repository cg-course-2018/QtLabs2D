#include "stdafx.h"

#include <QtWidgets/QApplication>
#include <libplatform/libplatform.h>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	platform::RenderWindowOptions opts = { 800, 600 };

	platform::RenderWindow window(opts);

	// TODO: window.setScene(std::move(scene));
	window.show();

	return app.exec();
}
