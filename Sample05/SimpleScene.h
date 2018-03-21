#pragma once
#include <libplatform/IRenderScene.h>
#include "SimpleScene.cpp"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <libplatform/libplatform.h>
#include <iostream>

class SimpleScene : public platform::IRenderScene
{
public:
	SimpleScene();

	void initialize() final;
	void update(float deltaSeconds) final;
	void redraw(unsigned width, unsigned height) final;
};