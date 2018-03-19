#include "stdafx.h"
#include <PoolTableApp\PoolTableScene.h>
#include <QtWidgets/QApplication>
#include <libplatform/libplatform.h>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	platform::RenderWindowOptions opts = { 800, 600 };

	platform::RenderWindow window(opts);

	auto scene = std::make_unique<PoolTableScene>(Rect2F(0, 0, opts.width, opts.height));
	window.setScene(std::move(scene));
	window.show();

	return app.exec();
}
