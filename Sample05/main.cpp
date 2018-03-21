#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <libplatform/libplatform.h>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	try
	{
		// Окно размером 800x600, используем OpenGL Core Profile
		platform::RenderWindowOptions opts = { 800, 600, true };
		platform::RenderWindow window(opts);

		window.show();
		return app.exec();
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		QMessageBox::warning(
			nullptr,
			QLatin1String("Sample05"),
			QString::fromUtf8(ex.what()));
	}
	window.setScene(std::make_unique<SimpleScene>());
}