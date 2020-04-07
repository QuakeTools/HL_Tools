#include <QMessageBox>

#include "game/World.hpp"
#include "graphics/Renderer.hpp"

#include "ui/ViewerWindow.hpp"

namespace ui
{
ViewerWindow::ViewerWindow(QWidget* parent)
	: QOpenGLWindow(UpdateBehavior::NoPartialUpdate)
{
	_widget = QWidget::createWindowContainer(this, parent);
}

ViewerWindow::~ViewerWindow()
{
	makeCurrent();

	_renderer.reset();
}

void ViewerWindow::initializeGL()
{
	try
	{
		_renderer = std::make_unique<graphics::Renderer>(this);
	}
	catch (const std::runtime_error& e)
	{
		QMessageBox::critical(_widget, "Fatal Error", QString("A fatal error occurred while initializing the renderer:\n") + e.what());
		throw;
	}
}

void ViewerWindow::resizeGL(int w, int h)
{
	_renderer->OnWindowResized(w, h);
}

void ViewerWindow::paintGL()
{
	_renderer->Render(_world);
}
}
