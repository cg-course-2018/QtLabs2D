#include "stdafx.h"

#include "LightingScene.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <iostream>
#include <libplatform/libplatform.h>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	platform::CatchAndDisplay([&] {
		// ���� �������� 800x600, ���������� OpenGL Core Profile � multisampling �� ��������� 8
		QSurfaceFormat format;
		format.setVersion(3, 3);
		format.setSamples(8);
		format.setProfile(QSurfaceFormat::CoreProfile);
		format.setOptions(QSurfaceFormat::DebugContext);
		format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
		platform::RenderWindowOptions opts = { 800, 600, format };

		platform::RenderWindow window(opts);
		window.setScene(std::make_unique<LightingScene>());

		window.show();
		return app.exec();
	});
}
