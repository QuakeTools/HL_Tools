#include "ui/ViewerWindow.h"

namespace ui
{
ViewerWindow::ViewerWindow(QWidget* parent)
	: QOpenGLWindow(UpdateBehavior::NoPartialUpdate)
{
	_widget = QWidget::createWindowContainer(this, parent);
}
}
